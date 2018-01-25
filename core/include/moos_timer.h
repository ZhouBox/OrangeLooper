#ifndef MOOS_TIMER_H
#define MOOS_TIMER_H


#include "moos_defines.h"
#include "moos_signal.h"
#include "moos_guard.h"
#include "moos_object.h"
#include "moos_logger.h"


DEFINE_NAMESPACE_MOOS_BEGIN


enum TIMER_TYPE
{
    SINGLE,
    REPEAT
};


class MoosTimer : public MoosObject
{
public:
    MoosTimer()
    : MoosObject()
    , m_ms(0)
    , m_guard(NULL)
    , m_type(REPEAT)
	, m_isStart(false)
    
    {
        
    }
    
    ~MoosTimer()
    {
        if (m_guard) {
            delete m_guard;
            m_guard = NULL;
        }
		m_isStart = true;
    }
    
    void start(int ms, TIMER_TYPE type_ = REPEAT)
    {
        m_type = type_;
        m_ms = ms;
        if (m_guard) {
            delete m_guard;
        }
        m_guard = new MoosGuard();
        MoosTaskBase* _task = new MoosDelayTask(std::bind(&MoosTimer::tick, this, *m_guard));
        _task->setTtl(m_ms);
        auto _looper = eventLooper();
        _looper->enqueue(_task);
		m_isStart = true;
    }

	void start()
	{
		start(m_ms);
	}

	bool isStrat() const
	{
		return m_isStart;
	}
    
    void stop()
    {
		m_isStart = false;
		delete m_guard;
		m_guard = NULL;
    }
    
    void setInterval(int ms)
    {
        m_ms = ms;
    }
    
    MOOS_SIGNAL() timeout;
    
protected:
    void tick(MoosGuard guard)
    {
		if (!m_isStart) {
			return;
		}
        if (guard) {
            MOOS_EMIT(timeout);
		}
		else {
			return;
		}
        if (m_type == REPEAT && m_isStart && m_guard) {
             MoosTaskBase* _task = new MoosDelayTask(std::bind(&MoosTimer::tick, this, *m_guard));
             _task->setTtl(m_ms);
             auto _looper = eventLooper();
             _looper->enqueue(_task);
        }
    }
    

private:
    int m_ms;
    MoosGuard* m_guard;
    TIMER_TYPE m_type;
	bool m_isStart;
    
};






DEFINE_NAMESPACE_MOOS_END




#endif // MOOS_TIMER_H
