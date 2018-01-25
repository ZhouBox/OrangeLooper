#include "../core/include/moos_includes.h"

#include <stdlib.h>


class ThreadTest : public nccloud::MoosThread
{
public:
	MOOS_SIGNAL(int, int) m_addSignal;


	bool threadRun()
	{
		MOOS_EMIT(m_addSignal, rand() % 100, rand() % 100);
		std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 10) * 1000));
		return true;
	}
};

class Test : public nccloud::MoosObject
{
public:
	void add(int a, int b)
	{
		MOOS_DEBUG_LOG(a, '+', b, '=', a + b);
	}

};

class Test1 : public nccloud::MoosObject
{
public:
	void reduce(int a, int b)
	{
		MOOS_DEBUG_LOG(a, '-', b, '=', a -b);
	}
};


int main()
{
	ThreadTest tt;
	Test t;
	nccloud::MoosThread tt1;
	Test1 t1;
	tt1.start();
	t.moveToThread(&tt1);

	MOOS_CONNECT(tt.m_addSignal, &t, &Test::add);
	MOOS_CONNECT(tt.m_addSignal, &t1, &Test1::reduce);
	tt.start();

	return MOOS_EXEC();
}
