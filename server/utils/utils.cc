#include "utils.h"

namespace server
{

    /**
     * @brief 获取线程编号 通过系统调用拿取到当前线程的编号
     */
    pid_t GetThreadId()
    {
        return syscall(SYS_gettid);
    }

    /**
     * @brief 获取协程编号
     */
    uint32_t GetFiberId()
    {
        // TODO: 实现协程的获取
        return 1000;
    }
}