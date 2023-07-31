#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <cstddef>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include "../utils/noncopyable.h"

namespace server
{

    /**
     * 信号量
     */
    class Semaphore
    {
    public:
        Semaphore();
        ~Semaphore();

        void wait();
        void notify();

    private:
        sem_t m_sem;
    };

    /**
     * 范围锁实现类
     */
    template <class T>
    class ScopedLockImpl
    {
    public:
        explicit ScopedLockImpl(T *t) : m_lock(t)
        {
            lock();
        }
        ~ScopedLockImpl()
        {
            unlock();
        }

        /**
         * 加锁
         */
        void lock()
        {
            if (!isLock)
            {
                m_lock->lock();
                isLock = true;
            }
        }

        /**
         * 解锁
         */
        void unlock()
        {
            if (isLock)
            {
                m_lock->unLock();
                isLock = false;
            }
        }

    private:
        T *m_lock;            // 锁变量
        bool isLock = false; // 是否上锁
    };

    /**
     * 读范围锁
     */
    template <class T>
    class ReadScopedLockImpl
    {
    public:
        ReadScopedLockImpl(T *t) : m_lock(t)
        {
            lock();
        }
        ~ReadScopedLockImpl()
        {
            unlock();
        }

        /**
         * 加锁
         */
        void lock()
        {
            if (!isLock)
            {
                m_lock->rdLock();
                isLock = true;
            }
        }

        /**
         * 解锁
         */
        void unlock()
        {
            if (isLock)
            {
                m_lock->unLock();
                isLock = false;
            }
        }

    private:
        T *m_lock;            // 锁变量
        bool isLock = false; // 是否上锁
    };

    /**
     * 写范围锁
     */
    template <class T>
    class WriteScopedLockImpl
    {
    public:
        WriteScopedLockImpl(T *t) : m_lock(t)
        {
            lock();
        }
        ~WriteScopedLockImpl()
        {
            unlock();
        }

        /**
         * 加锁
         */
        void lock()
        {
            if (!isLock)
            {
                m_lock->wrLock();
                isLock = true;
            }
        }

        /**
         * 解锁
         */
        void unlock()
        {
            if (isLock)
            {
                m_lock->unLock();
                isLock = false;
            }
        }

    private:
        T m_lock;            // 锁变量
        bool isLock = false; // 是否上锁
    };

    /**
     * 互斥量锁
     */
    class Mutex : NonCopyAble
    {
    public:
        typedef ScopedLockImpl<Mutex> Lock;
        Mutex();
        ~Mutex();
        void lock();
        void unLock();

    private:
        pthread_mutex_t m_mutex; // 线程互斥量
    };

    /**
     * 读写锁
     */
    class ReadWriteLock : NonCopyAble
    {
    public:
        typedef ReadScopedLockImpl<ReadWriteLock> RdLock;
        typedef WriteScopedLockImpl<ReadWriteLock> WrLock;
        ReadWriteLock();
        ~ReadWriteLock();

        void rdLock();
        void wrLock();
        void unLock();

    private:
        pthread_rwlock_t m_lock;
    };

    /**
     * 自旋锁
     */
    class SpinLock : NonCopyAble
    {
    public:
        typedef ScopedLockImpl<SpinLock> Lock;
        SpinLock();
        ~SpinLock();

        void lock();
        void unLock();

    private:
        pthread_spinlock_t m_lock;
    };

}

#endif
