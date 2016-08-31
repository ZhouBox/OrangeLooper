#include "moos_looper.h"

DEFINE_NAMESPACE_MOOS_BEGIN


std::map<std::thread::id, MoosLooper*> MoosLooper::m_loopers;

DEFINE_NAMESPACE_MOOS_END
