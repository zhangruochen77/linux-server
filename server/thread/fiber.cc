#include "fiber.h"

namespace server
{
    server::Logger::ptr g_log = SIG_LOG_ROOT();

    static thread_local Fiber *s_main_fiber = nullptr;          // 当前协程主协程
    static thread_local Fiber::ptr s_thread_fiber = nullptr;    // 当前线程中占据cpu的协程
    static thread_local std::atomic<uint64_t> s_fiber_count(0); // 当前线程中协程数量
    static thread_local std::atomic<uint64_t> s_fiber_id(0);    // 当前线程中线程自增id

    server::ConfigVar<uint32_t>::ptr g_stack_size = server::Config::Lookup<uint32_t>("fiber.stack_size", 128 * 1024, "fiber stack size");

    /**
     * @brief 堆区资源控制类
     */
    class MallocAllocator
    {
    public:
        static void *Molloc(size_t size)
        {
            return ::malloc(size);
        }

        static void Free(void *ptr)
        {
            if (nullptr != ptr)
            {
                ::free(ptr);
            }
        }
    };

    /**
     * @brief 初始化一个协程
     * @param cb 协程回调函数
     * @param stacksize 开辟协程栈大小
     * @param user_caller 是否在主协程上调度
     */
    Fiber::Fiber(fiber_cb cb, size_t stacksize, bool use_caller) : m_cb(cb)
    {
        size_t size = stacksize <= 0 ? g_stack_size->getValue() : stacksize; // 设置开辟空间大小，没有自定义大小 那么利用配置默认大小
        this->m_stack = MallocAllocator::Molloc(size);
        this->m_stacksize = size;

        GetMain();
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_size = size;
        m_ctx.uc_stack.ss_sp = m_stack;
        ASSERT_MSG(!(getcontext(&m_ctx)), "get context error");
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
        ++s_fiber_count;
        m_id = ++s_fiber_id;

        this->m_state = State::READY; // 初始化完毕转换为准备状态
    }

    /**
     * @brief 析构 清理自身垃圾 开辟栈空间清除
     */
    Fiber::~Fiber()
    {
        std::cout << "~Fiber exec " << m_id << std::endl;
        if (this->m_stack)
        {
            m_ctx.uc_stack.ss_sp = nullptr;
            MallocAllocator::Free(this->m_stack);
            m_stack = nullptr;
        }

        m_state = State::TERM;

        // 清除主协程信息
        if (1 == --s_fiber_count)
        {
            delete s_main_fiber;
            s_main_fiber = nullptr;
            s_thread_fiber = nullptr;
        }
    }

    /**
     * @brief 重置协程执行函数 非执行状态 即可重置
     */
    void Fiber::reset(fiber_cb cb)
    {
        ASSERT_MSG(!m_stack, "stack is null");
        ASSERT_MSG(!(State::HOLD == m_state || State::EXEC == m_state), "fiber execing!");

        m_state = State::INIT;
        m_cb = cb;
        m_ctx.uc_stack.ss_sp = m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;

        ASSERT_MSG(!(getcontext(&m_ctx)), "get context error");
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
        m_state = State::READY;
    }

    /**
     * @brief 切换当前协程到执行状态
     */
    void Fiber::swapIn()
    {
        Fiber *main = GetMain();
        s_thread_fiber = shared_from_this();
        m_state = State::EXEC;
        std::cout << "swap in count = " << s_thread_fiber.use_count() << std::endl;
        if (swapcontext(&main->m_ctx, &m_ctx))
        {
            ERROR(g_log)
                << "swap in error";
        }
    }

    /**
     * @brief 当前协程切换到后台执行
     */
    void Fiber::swapOut()
    {
        Fiber *main = GetMain();
        std::cout << "swap out count = " << s_thread_fiber.use_count() << std::endl;
        s_thread_fiber = nullptr;
        m_state = State::HOLD;
        if (swapcontext(&m_ctx, &main->m_ctx))
        {
            std::cout << "swap error" << std::endl;
        }
    }

    /**
     * @brief 获取当前进程执行协程
     */
    Fiber::ptr Fiber::GetThis()
    {
        if (s_thread_fiber)
        {
            return s_thread_fiber;
        }

        return GetMain()->shared_from_this();
        return nullptr;
    }

    /**
     * @brief 设置当前线程运行协程
     */
    void Fiber::SetThis(Fiber::ptr ptr)
    {
        s_thread_fiber = ptr;
    }

    /**
     * @brief 当前协程切换到后台 转换为 READY 状态
     */
    void Fiber::YieldToReady()
    {
        Fiber *main = GetMain();
        Fiber::ptr cur = GetThis();
        s_thread_fiber = nullptr;
        cur->m_state = State::READY;
        swapcontext(&cur->m_ctx, &main->m_ctx);
    }

    /**
     * @brief 当前协程协会到后台 转换为 HOLD 状态
     */
    void Fiber::YieldToHold()
    {
        Fiber *main = GetMain();
        Fiber::ptr cur = GetThis();
        s_thread_fiber = nullptr;
        cur->m_state = State::HOLD;
        swapcontext(&cur->m_ctx, &main->m_ctx);
    }

    /**
     * @brief 获取当前协程协程总数
     */
    uint64_t Fiber::TotalFiber()
    {
        return s_fiber_count.load();
    }

    /**
     * @brief 协程执行完毕调用 返回到主协程
     */
    void Fiber::MainFunc()
    {
        Fiber *main = GetMain();
        std::cout << "main func count = " << s_thread_fiber.use_count() << std::endl;
        s_thread_fiber->m_state = State::EXEC;

        try
        {
            s_thread_fiber->m_cb();
            s_thread_fiber->m_cb = fiber_cb{};
            s_thread_fiber->m_state = State::TERM;
        }
        catch (std::exception &e)
        {
            s_thread_fiber->m_state = State::EXCEPT;
            ERROR(g_log)
                << " fiber "
                << s_thread_fiber->m_id
                << "exec by exception:"
                << e.what()
                << std::endl
                << server::BackTraceToString();
        }

        swapcontext(&s_thread_fiber->m_ctx, &main->m_ctx);
        ASSERT_MSG(false, "can not access area");
    }

    /**
     * @brief 获取当前协程编号
     */
    uint64_t Fiber::GetFiberId()
    {
        if (s_thread_fiber)
        {
            return s_thread_fiber->m_id;
        }
        else
        {
            return GetMain()->m_id;
        }
    }

    /**
     * @brief 获取当前线程主协程 如果主协程还不存在 那么创建一个主协程 走私有构造
     */
    Fiber *Fiber::GetMain()
    {
        if (s_main_fiber)
        {
            return s_main_fiber;
        }

        s_main_fiber = new Fiber;
        return s_main_fiber;
    }

    /**
     * @brief 设置当前线程主协程 原有的没有关联 自动走析构
     */
    void Fiber::SetMain(Fiber *fiber)
    {
        if (!fiber)
        {
            s_main_fiber = nullptr;
            s_thread_fiber = nullptr;
        }
        else
        {
            s_main_fiber = fiber;
        }
    }

    /**
     * @brief 提供给主协程默认构建使用
     */
    Fiber::Fiber()
    {
        m_ctx.uc_link = nullptr; // 主协程 执行完毕就结束
        ASSERT_MSG(!(getcontext(&m_ctx)), "get context error");
        m_state = State::EXEC;
        ++s_fiber_count;
        m_id = ++s_fiber_id;
    }
}