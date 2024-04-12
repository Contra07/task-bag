#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <string>
#include <queue>
#include <chrono>

void qSortStandart(int* a, int size) {
	long i = 0;
	long j = size - 1;
	int temp, p;
	p = a[size >> 1];
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
		qSortStandart(a, j + 1);
	}
	if (size > i)
	{
		qSortStandart(a + i, size - i);
	}
}


void qSortOMP(int* a, int size, int limit)
{
	long i = 0;
	long j = size - 1;
	int temp, p;
	p = a[size >> 1];
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

	#pragma omp parallel sections
	{
		#pragma omp section
		if (j > 0)
		{
			if (j + 1 > limit)
			{
				qSortOMP(a, j + 1, limit);
			}
			else
			{
				qSortStandart(a, j + 1);
			}
		}
		#pragma omp section
		if (size > i)
		{
			if (size - i > limit)
			{
				qSortOMP(a+i, size - i, limit);
			}
			else
			{
				qSortStandart(a+i, size - i);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	int n{ std::stoi(argv[1]) };
	int p{ std::stoi(argv[2]) };
	int t = n / p;
	
	//std::cout << p;
	
	//omp_display_env(0);
	omp_set_nested(true);
	// �������������
	int* arrayPosl = new int[n];
	int* arrayParall = new int[n];

	for (int i = 0; i < n; i++) {
		int r = rand() % n;
		arrayPosl[i] = r;
		arrayParall[i] = r;
	}

	// ����� ������������ �������
	// std::cout << "\nArray(parral)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	std::cout << arrayParall[i] << " ";
	// }
	// std::cout << '\n';


	// ������� �������������
	auto startParall = std::chrono::steady_clock::now();
	qSortOMP(arrayParall, n,t);
	auto stopParall = std::chrono::steady_clock::now();
	auto durationParall = std::chrono::duration_cast<std::chrono::nanoseconds>(stopParall - startParall);

	// ����� ���������� �������������

	// cout << "\nbag duration = " << bag.duration() << " sec\n";

	std::cout << "\nparrallel duration = " << durationParall.count() << " nanosec\n";

	// cout << (durationParall.count()) << " ";

	//// ��������� ��� ������� N
	// std::cout << "\nArray(parallel)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	std::cout << arrayParall[i] << " ";
	// }
	// std::cout << '\n';

	// ����� ������������ �������
	// std::cout << "\nArray(posl)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	std::cout << arrayPosl[i] << " ";
	// }
	// std::cout << '\n';

	// omp_set_num_threads(1);
	// ���������������� 
	auto startPosl = std::chrono::steady_clock::now();
	qSortStandart(arrayPosl, n);
	auto stopPosl = std::chrono::steady_clock::now();
	auto durationPosl = std::chrono::duration_cast<std::chrono::nanoseconds>(stopPosl - startPosl);

	// ����� ���������� ����������������� 

	std::cout << "\nposl duration = " << durationPosl.count() << " nanosec\n";
	// cout << durationPosl.count() << " ";

	// ��������� ��� ������� N
	// std::cout << "\nArray(parallel)=\n";
	// for (int i = 0; i < n; i++)
	// {
	// 	std::cout << arrayPosl[i] << " ";
	// }
	// std::cout << '\n';

	std::cout << "\nspeedup = " << ((double)durationPosl.count() / (double)durationParall.count());
	// cout << ((double)durationPosl.count() / (double)durationParall.count()) << " ";

	// int i = 0;
	// while (i < n-1 && arrayParall[i] <= arrayParall[i+1])
	// {
	// 	i++;
	// }
	
	// std::cout << "\n" << (i == n-1) << "\n";

	delete[] arrayPosl;
	delete[] arrayParall;

	return 0;
}