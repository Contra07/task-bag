#include "tbag.h"
#include <stdio.h>

#include <iostream>
#include <queue>
#include <chrono>
using namespace std;
using namespace std::chrono;

// параллельное умножение матриц
// с использованием портфеля задач

const int P = 2; // число рабочих процессов (не используется в MPI)
const int N = 20;
const int T = 10010;
int arr[N];
int arrP[N];
struct mytask{
	int* a;
	int size;
	mytask(int * a1, int s1)
	{
		a = a1;
		size = s1;
	}
	mytask(){}
};

struct mybag{
	queue<mytask> taskQueue;
};

void qSort0(queue<mytask> * taskQueue, int* a, int size){
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
		if(j+1 < T)
		{
			taskQueue->push(mytask(a,(int)j+1));
		}
		else
		{
			qSort0(taskQueue,a,j+1);
		}
	}
	if (size > i)
	{
		if(size-i < T)
		{
			taskQueue->push(mytask(a+i,(int)(size-i)));
		}
		else
		{
			qSort0(taskQueue,a+i,size-i);
		}
	}
}

void qSort(queue<mytask> * taskQueue, int* a, int size){
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
		taskQueue->push(mytask(a,(int)(j+1)));
		//qSort(a,j+1);
	}
	if (size > i)
	{
		taskQueue->push(mytask(a+i,(int)(size-i)));
		//Sort(a+i, size-i);
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

		mytask t; // номер вычисляемой строки
	};

public:
	TaskBag(int num_prc, int argc, char *argv[]) : TBag(num_prc, argc, argv)
	{
		for(int i=0; i<N; i++){
			int r = rand() % N;
			arr[i]=r;
			arrP[i]=r;
		}
		// qSort0(&taskQueue, arr, N);
		taskQueue.push(mytask(arr,N));
	}
	virtual ~TaskBag() {}
	TBag::Task *createTask() { return new TaskBagTask; }

	queue<mytask> taskQueue;

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
		qSort(&taskQueue, mt->t.a,mt->t.size );
	}

	int cur; // номер текущей строки в матрице С
};

int main(int argc, char *argv[])
{
	TaskBag bag(P, argc, argv); // используем P рабочих процессов(не используется в MPI)

	// cout << "\nC(init)=\n";
	// for (int i = 0; i < N; i++)
	// {
	// 	cout << arr[i] << " ";
	// }
	// cout << '\n';
	
	// инициализация
	// for (int i = 0; i < N; i++)
	// {
	// 	arrP[i] = arr[i];
	// }

	// параллельное
	auto start = steady_clock::now();
	bag.run();
	auto stop = steady_clock::now();
	auto myduration = duration_cast<nanoseconds>(stop - start);
	cout << "\nspeedup = " << bag.speedup(); // 1-при логической отладке,
	// P-при параллельном выполении,
	// предсказываемое значение ускорения для данного P при дискретно-событийной эмуляции вычислений
	// внимание! режим эмуляции доступен только для ОС Windows

	cout << "\nduration = " << bag.duration() << " sec\n";
	cout << "\nmy duration = " << myduration.count() << endl;
	// вывод результата параллельного
	// отключить для больших N
	cout << "\nC(parallel)=\n";
	for (int i = 0; i < N; i++)
	{
		cout << arr[i] << " ";
	}
	cout << '\n';

	// последовательное 
	auto pstart = steady_clock::now();
	//qSort(arrP, N);
	auto pstop = steady_clock::now();
	auto pduration = duration_cast<nanoseconds>(pstop - pstart);
	// вывод результата последовательного 
	// отключить для больших N
	cout << "\nmy posl duration = " << pduration.count() << endl;
	// cout << "\nC(serial)=\n";
	// for (int i = 0; i < N; i++)
	// {
	// 	cout << arrP[i] << " ";
	// }
	cout << '\n';
	return 0;
}