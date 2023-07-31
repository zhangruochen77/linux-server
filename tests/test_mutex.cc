#include "server/thread/mutex.h"
#include "server/thread/thread.h"
#include "server/log/log.h"
#include <iostream>
#include <unistd.h>
#include <atomic>

static int val = 0;
static int tnum = 0;
static server::Mutex *m_mutex = new server::Mutex();
static std::atomic<int> end(0);

void fun1()
{
    server::Mutex::Lock lock(m_mutex);
    ++tnum;
    INFO(SIG_LOG_ROOT()) << " val = " << tnum;
    for (int j = 0; j < 1000; ++j)
    {
        ++val;
    }
    end.fetch_add(1);
}

int main()
{
    for (int i = 0; i < 1000; ++i)
    {
        server::Thread t(&fun1, "");
    }

    sleep(1);
    std::cout << " val = " << val << " tnum = " << tnum << " end = " << end.load() << std::endl;
    return 0;
}