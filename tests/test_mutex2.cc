// #include "server/thread/mutex.h"
// #include "server/log/log.h"
// #include "server/thread/thread.h"
// #include <iostream>
// #include <unistd.h>
// #include <atomic>
// #include <pthread.h>

// server::Logger::ptr g_logger = SIG_LOG_ROOT();
// server::Mutex *s_Mutex = new server::Mutex();

// static int val = 0;
// static int tnum = 0;
// static std::atomic<int> end(0);

// pthread_mutex_t mutex;

// void fuct01() {
//     pthread_mutex_lock(&mutex);
//     INFO(g_logger) << "thread test begin";
//     ++tnum;
//     for (int j = 0; j < 1000; ++j) {
//         server::Mutex::Lock lock(s_Mutex);
//         ++val;
//     }
//     // std::cout << val << std::endl;
//     pthread_mutex_unlock(&mutex);
//     end.fetch_add(1);

// }

// void fuct02() {

//     server::Mutex::Lock lock(s_Mutex);
//     // INFO(g_logger) << "thread test begin";
//     ++tnum;
//     for (int j = 0; j < 1000; ++j) {
//         ++val;
//     }
//     std::cout << val << std::endl;
//     end.fetch_add(1);

// }
// int main()
// {
//     std::cout << "****************** BEGIN ****************" << std::endl;

//     pthread_mutex_init(&mutex, 0);
//     // for (int i = 0; i < 20; ++i)
//     // {
//     //     server::Thread t(&fuct01, "");
//     //     pthread_mutex_destroy(&mutex);
//     // }

//     std::cout << "****************** BEGIN FUNC02 ****************" << std::endl;
//     for (int i = 0; i < 1000; ++i)
//     {
//         server::Thread t(&fuct02, "");
//     }
//     sleep(1);

//     std::cout << "****************** END ****************" << std::endl;
//     std::cout << " val = " << val << " tnum = " << tnum << " end = " << end.load() << std::endl;
//     return 0;
// }