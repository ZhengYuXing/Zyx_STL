#ifndef ZYX_LOCK_GUARD
#define ZYX_LOCK_GUARD

#include "NonCopyable.h"

namespace Zyx {

template <typename Mutex>
class LockGuard : NonCopyable
{
public:
    explicit LockGuard(Mutex& m_) : m(m_) 
    { 
        m.lock(); 
    }

    ~LockGuard()
    {
        m.unlock();
    }

private:
    Mutex& m;
};

}

#endif