/**
 * 自定义线程头文件
 */
#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>
#include <string>
#include <atomic>

#include "mutex.h"
#include "fiber.h"

namespace server
{
    /**
     * 自定义线程类
     */
    class Thread
    {
    public:
        typedef std::function<void()> thread_cb; // 自定义线程执行回调函数

        Thread(thread_cb cb, const std::string &name);
        ~Thread();
        /**
         * 等待当前线程执行完毕
         */
        void join();

        /**
         * 获取当前线程编号
         */
        static pid_t GetPid();

        /**
         * 获取当前线程名称
         */
        static const std::string GetName();

    private:
        Thread &operator=(const Thread &t) = delete; // 删除等值构造函数
        Thread(const Thread &t) = delete;            // 删除赋值构造函数
        Thread(const Thread &&t) = delete;           // 删除右值构造函数

        /**
         * 启动线程执行
         */
        static void *run(void *args);

    private:
        thread_cb m_cb;     // 线程执行回调函数
        pid_t m_pid;        // 线程编号
        std::string m_name; // 线程名称
        pthread_t m_thread; // 线程数据类型值
        Semaphore m_sem;
    };

}

#endif