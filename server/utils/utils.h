#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <cstdint>

namespace server
{

    /**
     * @brief 获取线程id
     */
    pid_t GetThreadId();

    /**
     * @brief 获取协程编号
     */
    uint32_t GetFiberId();
}

#endif