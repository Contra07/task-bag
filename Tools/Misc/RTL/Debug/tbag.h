//////////////////////////////////////////////////////////////
//	����: tbag.h											//
//	������ '�������� �����' ��� ���������� �������      	//
//	Copyright (c) 2010-2015 �������� ������ ������������	//
//////////////////////////////////////////////////////////////

#ifndef _TASK_BAG_RUN_TIME
#define _TASK_BAG_RUN_TIME

namespace TEMPLET {

#define ALLOC_SIZE 1024

class TBag{
public:
	class Task{
		friend class TBag;
	public:
		Task();
		virtual~Task();
		
		void send(void*,size_t);//��������� ����������/�������������� ���������
		void recv(void*,size_t);

		virtual void send_task()=0;//�������� ������ ��� ����������/�������������� ���������
		virtual void recv_task()=0;
		virtual void send_result()=0;
		virtual void recv_result()=0;
	private:
		void*  buf; //������ ������
		size_t size;//������ ������
		size_t cur; //�������� ��� ������
	};
public:
	TBag(int num_prc,int argc=0, char* argv[]=0){task=0;_duration=0.0;}
	virtual ~TBag(){}
	virtual Task* createTask()=0;
	
	void run();
	virtual bool if_job()=0;//���� �� ������ ��� ���������?
	virtual void put(Task*)=0;//��������� ���������� ������� ������ � ��������
	virtual void get(Task*)=0;//���������� ������ ��� ������� �� ��������
	virtual void proc(Task*)=0;//������� ������ � ������� ��������

	double speedup(){return 1;};
	double duration(){return _duration;};

private:
	Task* task;
	double _duration;
};

}
#endif
