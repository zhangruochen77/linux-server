#ifndef __FIBER_H__
#define __FIBER_H__

#include <memory>
#include <ucontext.h>
#include <functional>
#include <atomic>

#include "../log/log.h"
#include "../config/config.h"
#include "../utils/macro.h"

namespace server
{
    /**
     * 协程处理类
     */
    class Fiber : public std::enable_shared_from_this<Fiber>
    {
    public:
        typedef std::shared_ptr<Fiber> ptr;
        typedef std::function<void()> fiber_cb;

        enum State
        {
            // 初始化状态
            INIT,
            // 可执行状态
            READY,
            // 暂停状态
            HOLD,
            // 执行状态
            EXEC,
            // 结束状态
            TERM,
            // 异常状态
            EXCEPT
        };

        /**
         * @brief 初始化一个协程
         * @param cb 协程回调函数
         * @param stacksize 开辟协程栈大小
         * @param user_caller 是否在主协程上调度
         */
        Fiber(fiber_cb cb, size_t stacksize = 0, bool use_caller = false);

        /**
         * @brief 析构 清理自身垃圾
         */
        ~Fiber();

        /**
         * @brief 重置协程执行函数
         */
        void reset(fiber_cb cb);

        /**
         * @brief 切换当前协程到执行状态
         */
        void swapIn();

        /**
         * @brief 当前协程切换到后台执行
         */
        void swapOut();

        /**
         * @brief 获取当前协程编号
         */
        uint64_t getId() const { return this->m_id; }

        /**
         * @brief 获取协程状态
         */
        State getState() const { return this->m_state; }

    public:
        /**
         * @brief 获取当前线程主协程
         */
        static Fiber::ptr GetMain();

        /**
         * @brief 设置当前线程主协程
         */
        static void SetMain(Fiber::ptr &fiber);

        /**
         * @brief 获取当前执行协程
         */
        static Fiber::ptr GetThis();

        /**
         * @brief 设置当前线程运行协程
         */
        static void SetThis(Fiber::ptr ptr);

        /**
         * @brief 当前协程切换到后台 转换为 READY 状态
         */
        static void YieldToReady();

        /**
         * @brief 当前协程协会到后台 转换为 HOLD 状态
         */
        static void YieldToHold();

        /**
         * @brief 获取当前协程协程总数
         */
        static uint64_t TotalFiber();

        /**
         * @brief 协程执行完毕调用 返回到主协程
         */
        static void MainFunc();

        /**
         * @brief 获取当前协程编号
         */
        static uint64_t GetFiberId();

    private:
        Fiber(); // 默认构造函数私有化

    private:
        uint64_t m_id = 0;           // 协程id
        State m_state = State::INIT; // 协程状态
        size_t m_stacksize = 0;      // 栈空间大小
        ucontext_t m_ctx;            // 协程上下文
        fiber_cb m_cb;               // 协程回调函数
        void *m_stack = nullptr;     // 栈空间指针
    };
}

#endif