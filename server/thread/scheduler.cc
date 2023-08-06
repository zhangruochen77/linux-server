/**
 * 协程调度器基本实现类
 */

#include "scheduler.h"

namespace server
{

    static server::Logger::ptr g_logger = SIG_LOG_ROOT();

    /**
     * @brief 初始化调度器属性
     * @param 线程具有数量
     * @name 调度器名称
     */
    Scheduler::Scheduler(uint32_t threadSize = 1, const std::string &name = "")
        : m_threadSize(threadSize), m_name(name)
    {
    }

    Scheduler *Scheduler::GetThis() {}

    server::Fiber *SchedulerGetMainFiber()
    {
    }

    /**
     * @brief 启动调动器
     */
    void Scheduler::start()
    {
        Lock lock(&m_lock);
        if (m_stop)
        {
            ASSERT_MSG(false, "can not start the schedule, because schedule is stop");
        }

        for (uint32_t i = 0; i < m_threadSize; ++i)
        {
            server::Thread t(std::bind(&Scheduler::run, this), m_name + std::to_string(i));
            m_threads.push_back(t);
        }
    }

    /**
     * @brief 执行任务
     */
    void Scheduler::run()
    {
        FiberAndCb *fc = nullptr;
        while (true)
        {
            {
                Lock lock(&m_lock);
                if (m_task.empty())
                {
                    lock.unlock();
                    if (exitByStop())
                    {
                        break;
                    }

                    idle();
                    continue;
                }

                for (auto e = m_task.begin(); e != m_task.end(); ++e)
                {
                    if (e->m_threadId != -1 && e->m_threadId != Thread::GetPid())
                    {
                        m_notice = true;
                        continue;
                    }

                    fc = &(*e);
                    m_task.erase(e);
                    break;
                }
            }

            // 是否通知其他线程执行任务 准确来说是唤醒
            if (m_notice)
            {
                notice();
            }

            // 当前线程没有执行任务 应该陷入沉睡
            if (!fc)
            {
                idle();
                continue;
            }

            // 调度执行协程任务
            if (fc->m_fiber)
            {
                while (Fiber::EXCEPT != fc->m_fiber->getState() || Fiber::TERM != fc->m_fiber->getState())
                {
                    fc->m_fiber->swapIn();
                }
            }
            // 创建一个协程 执行任务
            else if (fc->m_cb)
            {
                Fiber::ptr fb = std::make_shared<Fiber>(fc->m_cb);
                while (Fiber::EXCEPT != fb->getState() || Fiber::TERM != fb->getState())
                {
                    fc->m_fiber->swapIn();
                }
            }
            // 出现错误 不存在协程也不存在任务
            else
            {
                ASSERT_MSG(fc, "can not exec task, because it is don't have fiber or cb");
            }

            fc = nullptr;
        }
    }

    /**
     * @brief 停止调度器
     */
    void Scheduler::stop()
    {
        m_stop = true;
        notice();
    }

    /**
     * @brief 是否退出
     */
    bool Scheduler::exitByStop()
    {
        return m_stop && m_task.empty();
    }

    /**
     * @brief 添加一个调度任务
     */
    void Scheduler::schedule(FiberAndCb fac)
    {
        Lock lock(&m_lock);
        if (!m_stop)
        {
            m_task.push_back(fac);
        }
        else
        {
            ASSERT_MSG(false, "can not exec task, because schedule is stop");
        }
    }

    /**
     * @brief 通知执行
     */
    void Scheduler::notice()
    {
        INFO(g_logger) << "scheduler notice";
    }

    /**
     * @brief 无任务时执行
     */
    void Scheduler::idle()
    {
        INFO(g_logger) << "scheduler idel";
        sleep(1);
    }

    /**
     * @brief 析构函数
     */
    Scheduler::~Scheduler()
    {
        INFO(g_logger) << "scheduler ~ exec";
    }
}