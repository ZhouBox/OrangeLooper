#include "../core/include/moos_includes.h"

#include <stdlib.h>


class ThreadTest : public Moos::Thread
{
public:
	MOOS_SIGNAL(int, int) m_addSignal;_


	bool threadRun()
	{
		MOOS_EMIT(m_addSignal, rand() % 100, rand() % 100);
		std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 10) * 1000));
		return true;
	}
};

class Test : public Moos::MoosObject
{
public:
	void add(int a, int b)
	{
		MOOS_DEBUG_LOG(a, '+', b, '=', a + b);
	}

};


int main()
{
	ThreadTest tt;
	Test t;
	MOOS_CONNECT(tt.m_addSignal, &t, &Test::add);
	tt.start();
	return MOOS_EXEC();
}