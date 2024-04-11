/////////////////////////////////////////////////////////////////////////////
// файл: tbag.h                                                            //
// Модуль имитационного моделирования для Windows API                      //
// Copyright (c) 2014 Сергей Востокин, Влад Мартынюк                       //
/////////////////////////////////////////////////////////////////////////////

#ifndef _TASK_BAG_RUN_TIME
#define _TASK_BAG_RUN_TIME

#include <vector>
#include <queue>

namespace TEMPLET {

class TBag{
public:
	class Task{
		friend class TBag;
	public:
		Task(){};
		virtual~Task(){}
		
		void send(void*,size_t){}//интерфейс сохранения/восстановления состояния
		void recv(void*,size_t){}

		virtual void send_task()=0;//обратные вызовы для сохранения/восстановления состояния
		virtual void recv_task()=0;
		virtual void send_result()=0;
		virtual void recv_result()=0;
	private:
		int offset;
	};

	enum EventType { DEFAULT, IF_JOB, PUT, GET, PROC, MASTER };

	class Evt{
	friend class Cmp;
	friend class TBag;

	public:
		Evt(): _type(DEFAULT), _plan_time(0.0), _planed(false), _if_job(false) {}

	private:
		double _plan_time;
		bool _planed;
		EventType _type;
		int _offset;
		bool _if_job;
	};

	class Cmp{public:bool operator()(const Evt*t1,const Evt*t2)const{return t1->_plan_time > t2->_plan_time;}};

public:
	TBag(int num_prc,int argc=0, char* argv[]=0);
	virtual ~TBag();
	virtual Task* createTask()=0;
	
	//Запуск портфеля
	void run();
	//Есть ли задачи для обработки?
	virtual bool if_job()=0;
	//Помещение результата решения задачи в портфель
	virtual void put(Task*)=0;
	//Извлечение задачи для решения из портфеля
	virtual void get(Task*)=0;
	//Решение задачи в рабочем процессе
	virtual void proc(Task*)=0;

	//Вернуть оценку ускорения 
	double speedup();
	//Вернуть длительность счета
	double duration();
	
private:
	bool if_job(double&); //обертки методов, возвращающие время
	void put(Task*, double&);
	void get(Task*, double&);
	void proc(Task*, double&);

private:
	Task **_tasks;
	Evt *_events;
	Evt _event;
	int _num_proc;
	bool _busy;

private:
	void plan(Evt*,double);
	std::priority_queue<Evt*, std::vector<Evt*>, Cmp> _calendar;
	std::queue<Task*> _bids; // заявкии
	std::queue<Task*> _results; // результаты обработки задачи
	double _curr_time;
	double _user_time;
	double _duration;
};

}
#endif
