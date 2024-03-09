//////////////////////////////////////////////////////////////
//	����: tbag.cpp											//
//	������ '�������� �����' ��� ���������� �������      	//
//	Copyright (c) 2010-2015 �������� ������ ������������	//
//////////////////////////////////////////////////////////////

#include "tbag.h"

#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>

namespace TEMPLET {

TBag::Task::Task()
{
	buf=malloc(ALLOC_SIZE);
	size=ALLOC_SIZE;
	cur=0;
}

TBag::Task::~Task()
{
	free(buf);
}
		
void TBag::Task::send(void*p,size_t s)
{
	while(cur+s>size){buf=realloc(buf,size+ALLOC_SIZE);size+=ALLOC_SIZE;}
	memmove((char*)buf+cur,p,s);cur+=s;
}

void TBag::Task::recv(void*p,size_t s)
{
	memmove(p,(char*)buf+cur,s);cur+=s;
}

void TBag::run()
{
	task=createTask();
	
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1,t2;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t1);

	while(if_job()){
		size_t limit;//��� �������� ���������� ������� ������� ������ ��� ������
		get(task);/////////////////////////
		task->cur=0;task->send_task();
		limit=task->cur;
		task->cur=0;task->recv_task();
		if(limit!=task->cur){printf("������ ��� ���������� �������������� ������\n");exit(0);}
		proc(task);////////////////////////
		task->cur=0;task->send_result();
		limit=task->cur;
		task->cur=0;task->recv_result();
		if(limit!=task->cur){printf("������ ��� ���������� �������������� ����������\n");exit(0);}
		put(task);/////////////////////////
	}

	QueryPerformanceCounter(&t2);
	_duration=(double)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;
	DWORD tend=GetTickCount();
	
	delete task;
}

}