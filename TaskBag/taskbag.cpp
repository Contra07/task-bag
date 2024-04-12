#include "tbag.h"
#include <stdio.h>

#include <iostream>
#include <string>
#include <queue>
#include <chrono>
using namespace std;
using namespace std::chrono;

// параллельное умножение матриц
// с использованием портфеля задач

// const int P = 12; // число рабочих процессов (не используется в MPI)
// const int N = 50000000;
// const int T = N / P;
// int arr[N];
// int arrP[N];

struct array_task{
	int* array;
	int length;
	array_task(int * a1, int s1)
	{
		array = a1;
		length = s1;
	}
	array_task(){}
};

void qSortInitTasks(queue<array_task> * taskQueue, int* a, int size, int limit){
	long i = 0;
	long j = size-1;
	int temp, p;
	p = a[size>>1];
	do
	{
		while (a[i] < p)
		{
			i++;
		}
		while (a[j] > p)
		{
			j--;
		}
		if (i <= j)
		{
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
			j--;
		}
		
	} while (i <= j);

	if (j > 0)
	{
		if(j+1 > limit)
		{
			taskQueue->push(array_task(a,(int)j+1));
		}
		else
		{
			qSortInitTasks(taskQueue,a,j+1, limit);
		}
	}
	if (size > i)
	{
		if(size-i > limit)
		{
			taskQueue->push(array_task(a+i,(int)(size-i)));
		}
		else
		{
			qSortInitTasks(taskQueue,a+i,size-i, limit);
		}
	}
}

void qSortStandart(int* a, int size){
	long i = 0;
	long j = size-1;
	int temp, p;
	p = a[size>>1];
	do
	{
		while (a[i] < p)
		{
			i++;
		}
		while (a[j] > p)
		{
			j--;
		}
		if (i <= j)
		{
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
			j--;
		}
		
	} while (i <= j);
	
	if (j > 0)
	{
		qSortStandart(a,j+1);
	}
	if (size > i)
	{
		qSortStandart(a+i, size-i);
	}
}

class TaskBag : public TEMPLET::TBag
{
public:
	class TaskBagTask : public TBag::Task
	{
	public:
		TaskBagTask() : TBag::Task() {}
		virtual ~TaskBagTask() {}

		void send_task() {} // запись задачи в поток  (только для распределенной реализации)
		void recv_task() {} // четние задачи из потока (только для распределенной реализации)
		void send_result() {} // запись результата в поток (только для распределенной реализации)
		void recv_result() {} // чтение результата из потока (только для распределенной реализации)

		array_task t; // номер вычисляемой строки
	};

public:
	TaskBag(int num_prc, int argc, char *argv[], int* array, int array_length, int limit) : TBag(num_prc, argc, argv)
	{
		qSortInitTasks(&taskQueue, array, array_length, limit);
	}
	virtual ~TaskBag() {}
	TBag::Task *createTask() { return new TaskBagTask; }

	queue<array_task> taskQueue;

	bool if_job() 
	{ 
		return !taskQueue.empty(); 
	}
	void put(Task *t) { }
	void get(Task *t)
	{
		TaskBagTask *mt = (TaskBagTask *)t;
		mt->t = taskQueue.front();
		taskQueue.pop();
	}
	void proc(Task *t)
	{
		TaskBagTask *mt = (TaskBagTask *)t;
		qSortStandart(mt->t.array,mt->t.length );
	}

	int cur; // номер текущей строки в матрице С
};

int main(int argc, char *argv[])
{
	
	int n{std::stoi(argv[1])};
	int p{std::stoi(argv[2])};
	int t = n / p;
	
	// инициализация
	int* arrayPosl = new int[n];
	int* arrayParall = new int[n];

	for(int i=0; i<n; i++){
		int r = rand() % n;
		arrayPosl[i]=r;
		arrayParall[i]=r;
	}
	
	// вывод изначального массива
	// cout << "\nArray(init)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	cout << arrayParall[i] << " ";
	// }
	// cout << '\n';

	// подсчет параллельного
	auto startParall = steady_clock::now();
	TaskBag bag(p, argc, argv, arrayParall, n, t); // используем P рабочих процессов(не используется в MPI)
	bag.run();
	auto stopParall = steady_clock::now();
	auto durationParall = duration_cast<nanoseconds>(stopParall - startParall);

	// вывод результата параллельного

	// cout << "\nbag duration = " << bag.duration() << " sec\n";
	cout << "\nparrallel duration = " << durationParall.count() << " nanosec\n";
	// cout << (durationParall.count()) << " ";

	// отключить для больших N
	// cout << "\nArray(parallel)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	cout << arrayParall[i] << " ";
	// }
	// cout << '\n';

	// вывод изначального массива
	// cout << "\nArray(init)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	cout << arrayPosl[i] << " ";
	// }
	// cout << '\n';

	// последовательное 
	auto startPosl = steady_clock::now();
	qSortStandart(arrayPosl, n);
	auto stopPosl = steady_clock::now();
	auto durationPosl = duration_cast<nanoseconds>(stopPosl - startPosl);
	
	// вывод результата последовательного 

	cout << "\nposl duration = " << durationPosl.count() << " nanosec\n";
	// cout << durationPosl.count() << " ";

	// отключить для больших N
	// cout << "\nArray(parallel)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	cout << arrayPosl[i] << " ";
	// }
	// cout << '\n';

	cout << "\nspeedup = " << ((double)durationPosl.count() / (double)durationParall.count()) ;
	// cout << ((double)durationPosl.count() / (double)durationParall.count()) << " ";


	// int i = 0;
	// while (i < n-1 && arrayParall[i] <= arrayParall[i+1])
	// {
	// 	i++;
	// }
	
	// std::cout << "\n" << (i == n-1) << "\n";

	delete [] arrayPosl;
	delete [] arrayParall;
	
	return 0;
}