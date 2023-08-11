/**
 * 协程调度器
 */
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <list>
#include <atomic>

#include "fiber.h"
#include "mutex.h"
#include "thread.h"
#include "../log/log.h"

namespace server
{

    class Scheduler
    {
    public:
        typedef std::shared_ptr<Scheduler> ptr;        // 共享指针
        typedef Mutex LockType;                        // 定义所类型
        typedef server::ScopedLockImpl<LockType> Lock; // 定义范围锁

        /**
         * @brief 协程任务或者回调函数
         */
        class FiberAndCb
        {

        public:
            friend class Scheduler;
            FiberAndCb(server::Fiber::ptr fiber, pid_t threadId = -1)
                : m_fiber(fiber), m_threadId(threadId) {}

            FiberAndCb(std::function<void(void)> cb, pid_t threadId = -1)
                : m_cb(cb), m_threadId(threadId) {}

        private:
            FiberAndCb() {}

        private:
            server::Fiber::ptr m_fiber = nullptr;     // 协程任务
            std::function<void(void)> m_cb = nullptr; // 回调任务
            pid_t m_threadId;                         // 指定执行协程的线程编号
        };

        /**
         * @brief 初始化调度器属性
         * @param 线程具有数量
         * @name 调度器名称
         */
        Scheduler(uint32_t threadSize = 1, const std::string &name = "");

        /**
         * @brief 析构函数 部分数据的销毁需要交由子类处理
         */
        virtual ~Scheduler();

        /**
         * @brief 获取调度器名称
         */
        const std::string &getName() const { return m_name; }

        static Scheduler *GetThis();

        static server::Fiber *GetMainFiber();

        /**
         * @brief 启动调动器
         */
        void start();

        /**
         * @brief 停止调度器
         */
        void stop();

        /**
         * @brief 添加一个调度任务
         */
        void schedule(FiberAndCb fac);

        /**
         * @brief 是否退出
         */
        bool exitByStop();

    protected:
        /**
         * @brief 通知执行
         */
        virtual void notice();

        /**
         * @brief 无任务时执行
         */
        virtual void idle();
        /**
         * 每个线程所调度执行任务
         */
        void run();

    private:
        uint32_t m_threadSize = 0;                  // 线程总数量
        std::string m_name;                         // 调度器名称
        std::atomic<uint32_t> m_activeNum = {0};    // 活跃线程数量
        std::atomic<uint32_t> m_workNum = {0};      // 执行任务线程数量
        Mutex m_lock;                               // 锁变量
        std::vector<server::Thread::ptr> m_threads; // 线程池
        std::vector<FiberAndCb> m_task;             // 任务队列
        bool m_stop = false;                        // 调度器是否被停止
        bool m_notice = false;                      // 是否通知其他线程执行任务
    };
}

#endif