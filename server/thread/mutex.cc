#include "mutex.h"
#include <iostream>
#include "../utils/perror_utils.h"

namespace server
{
    Mutex::Mutex()
    {
        if (pthread_mutex_init(&m_mutex, nullptr))
        {
            throw std::logic_error("pthread_mutex_init error");
        }
    }
    Mutex::~Mutex()
    {
        if (pthread_mutex_destroy(&m_mutex))
        {
            err("pthread_mutex_destroy err");
        }
    }
    void Mutex::lock()
    {
        if (pthread_mutex_lock(&m_mutex))
        {
            throw std::logic_error("pthread_mutex_lock error");
        }
    }
    void Mutex::unLock()
    {
        if (pthread_mutex_unlock(&m_mutex))
        {
            throw std::logic_error("pthread_mutex_unlock error");
        }
    }

    ReadWriteLock::ReadWriteLock()
    {
        if (pthread_rwlock_init(&m_lock, nullptr))
        {
            throw std::logic_error("pthread_rwlock_init error");
        }
    }
    ReadWriteLock::~ReadWriteLock()
    {
        if (pthread_rwlock_destroy(&m_lock))
        {
            err("pthread_rwlock_destroy error");
        }
    }
    void ReadWriteLock::rdLock()
    {
        if (pthread_rwlock_rdlock(&m_lock))
        {
            throw std::logic_error("pthread_rwlock_rdlock error");
        }
    }
    void ReadWriteLock::wrLock()
    {
        if (pthread_rwlock_wrlock(&m_lock))
        {
            throw std::logic_error("pthread_rwlock_wrlock error");
        }
    }
    void ReadWriteLock::unLock()
    {
        if (pthread_rwlock_unlock(&m_lock))
        {
            throw std::logic_error("pthread_rwlock_unlock error");
        }
    }

    /**
     * 初始化自旋锁 设定启用范围为当前进程
     */
    SpinLock::SpinLock()
    {
        if (pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE))
        {
            throw std::logic_error("pthread_spin_init error");
        }
    }
    SpinLock::~SpinLock()
    {

        if (pthread_spin_destroy(&m_lock))
        {
            err("pthread_spin_destroy error");
        }
    }
    void SpinLock::lock()
    {
        if (pthread_spin_lock(&m_lock))
        {
            throw std::logic_error("pthread_spin_lock error");
        }
    }
    void SpinLock::unLock()
    {
        if (pthread_spin_unlock(&m_lock))
        {
            throw std::logic_error("pthread_spin_unlock error");
        }
    }

    /**
     * 信号量 线程之间通信
     */
    Semaphore::Semaphore()
    {
        if (sem_init(&m_sem, 0, 0))
        {
            throw std::logic_error("sem init error");
        }
    }
    Semaphore::~Semaphore()
    {
        if (sem_destroy(&m_sem))
        {
            err("sem destory error");
        }
    }
    void Semaphore::wait()
    {
        if (sem_wait(&m_sem))
        {
            throw std::logic_error("sem wait error");
        }
    }
    void Semaphore::notify()
    {
        if (sem_post(&m_sem))
        {
            throw std::logic_error("sem post error");
        }
    }
}