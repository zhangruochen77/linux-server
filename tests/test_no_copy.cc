#include <pthread.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();

private:
    pthread_mutex_t mu_;

    // No copying
    Mutex(const Mutex &) = delete;
    void operator=(const Mutex &) = delete;
};

class MutexLock
{
public:
    explicit MutexLock(Mutex *mu)
        : mu_(mu)
    {
        this->mu_->Lock();
    }
    ~MutexLock() { this->mu_->Unlock(); }

private:
    Mutex *const mu_;
    // No copying allowed
    MutexLock(const MutexLock &) = delete;
    void operator=(const MutexLock &) = delete;
};

static void PthreadCall(const char *label, int result)
{
    if (result != 0)
    {
        fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
    }
}

Mutex::Mutex() { PthreadCall("init mutex", pthread_mutex_init(&mu_, NULL)); }

Mutex::~Mutex() { PthreadCall("destroy mutex", pthread_mutex_destroy(&mu_)); }

void Mutex::Lock() { PthreadCall("lock", pthread_mutex_lock(&mu_)); }

void Mutex::Unlock() { PthreadCall("unlock", pthread_mutex_unlock(&mu_)); }

#define NUM_THREADS 10000

int num = 0;
Mutex mutex;

void *count(void *args)
{
    MutexLock lock(&mutex);
    num++;

    return nullptr;
}

int main()
{
    int t;
    pthread_t thread[NUM_THREADS];

    for (t = 0; t < NUM_THREADS; t++)
    {
        int ret = pthread_create(&thread[t], NULL, count, NULL);
        if (ret)
        {
            return -1;
        }
    }

    for (t = 0; t < NUM_THREADS; t++)
        pthread_join(thread[t], NULL);
    std::cout << num << std::endl;
    return 0;
}