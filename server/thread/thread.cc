#include "thread.h"
#include "../utils/utils.h"
#include "../utils/perror_utils.h"

namespace server
{
    /**
     * 定义当前线程所属的局部变量
     */
    static thread_local pid_t current_pid = 0;
    static thread_local std::string current_pname = "UNKNOW";

    /**
     * 获取当前线程编号
     */
    pid_t Thread::GetPid()
    {
        return current_pid;
    }

    /**
     * 获取当前线程名称
     */
    const std::string Thread::GetName()
    {
        return current_pname;
    }

    /**
     * 同步当前属性启动线程执行
     */
    void *Thread::run(void *args)
    {
        Thread *t = (Thread *)args;
        t->m_pid = GetThreadId();
        server::current_pid = t->m_pid;
        server::current_pname = t->m_name;
        if (pthread_setname_np(pthread_self(), t->m_name.substr(0, 15).c_str()))
        {
            if (errno == ERANGE)
            {
                err("pthread_setname_np:the length of the string specified pointed to by name exceeds the allowed limit");
            }
            else
            {
                err("pthread_setname_np: unknow error");
            }
        }

        t->m_cb();
    }

    /**
     * 初始化线程 创建线程调度执行
     */
    Thread::Thread(thread_cb cb, const std::string &name)
        : m_cb(cb), m_name(name)
    {
        if (m_name.empty())
        {
            m_name = "UNKNOW";
        }

        if (pthread_create(&m_thread, nullptr, &Thread::run, (void *)this))
        {
            if (EAGAIN == errno)
            {
                err("pthread_create: insufficient resources to create another thread");
            }
            else
            {
                err("pthread_create: cannot create another thread and the cause is unknow ");
            }
        }
    }

    /**
     * 线程销毁 将线程进行分离
     */
    Thread::~Thread()
    {
        if (m_thread)
        {
            if (pthread_detach(m_thread))
            {
                err("pthrea_detach: error");
            }
        }
    }

    /**
     * 等待当前线程执行完毕
     */
    void Thread::join()
    {
        if (m_thread)
        {
            if (pthread_join(m_thread, nullptr))
            {
                err("ptreahd_join: error");
            }
        }

        m_thread = 0; // 线程已经结束 设定其销毁
    }
}