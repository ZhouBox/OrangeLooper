#include "moos_looper.h"

DEFINE_NAMESPACE_MOOS_BEGIN


std::map<std::thread::id, MoosLooper*> MoosLooper::m_loopers;
std::mutex MoosLooper::m_mutex;

DEFINE_NAMESPACE_MOOS_END
