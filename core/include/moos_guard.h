#ifndef MOOS_GUARD_H
#define MOOS_GUARD_H


#include "moos_defines.h"

#include <atomic>

DEFINE_NAMESPACE_MOOS_BEGIN


class MoosGuard
{
  public:
    MoosGuard()
      : m_guard(new MoosGuardHelper)
      , m_isHost(true)
    {
    }
  
  ~MoosGuard()
  {
    if (m_isHost) {
      *m_guard->m_vaild = false;
    }
    
    if ((--(*m_guard->m_ref)) == 0) {
      delete m_guard;
    }
  }
  
  MoosGuard(const MoosGuard& other)
  {
    m_guard = other.m_guard;
    m_isHost = false;
    ++(*m_guard->m_ref);
  }
  
  MoosGuard& operator=(const MoosGuard& other)
  {
    m_guard = other.m_guard;
    m_isHost = false;
    ++(*m_guard->m_ref);
    return *this;
  }
  
  operator bool()
  {
    return m_guard->m_vaild;
  }
  
  private:
  struct MoosGuardHelper
  {
    MoosGuardHelper()
    {
      m_ref = new std::atomic_int(1);
      m_vaild = new std::atomic_bool(true);
    }
    
    ~MoosGuardHelper()
    {
      delete m_ref;
      delete m_vaild;
    }
    std::atomic_int *m_ref;
    std::atomic_bool *m_vaild;
  };
  
  MoosGuardHelper* m_guard;
  std::atomic_bool m_isHost;
  
};

template<typename T>
class MoosGuardPtr
{
  public:
  MoosGuardPtr(T* ptr_)
    :m_ptr(ptr_)
    ,m_guard(ptr_->guard())
  {
  }
  
  T* operator->()
  {
    return m_ptr;
  }
  
  T& operator*()
  {
    return *m_ptr;
  }
  
  operator bool()
  {
    return m_guard;
  }
  
  private:
  T* m_ptr;
  MoosGuard m_guard;
};





DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_GUARD_H
