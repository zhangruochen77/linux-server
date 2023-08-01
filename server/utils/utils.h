#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <cstdint>
#include <execinfo.h>
#include <string>
#include <vector>
#include <sstream>
#include <cxxabi.h>

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

    /**
     * @brief 获取堆栈信息
     * @param[out] traces 存放堆栈信息集合
     * @param size 单次存放堆栈层级数量
     * @param skip 跳过展示层数
     */
    void BackTrace(std::vector<std::string> &traces, size_t size, size_t skip);

    /**
     * @brief 获取堆栈信息以字符串形式
     * @param size 堆栈层数大小
     * @param skip 跳过展示层数
     * @param prefix 设置打印展示浅醉
     */
    std::string BackTraceToString(size_t size = 64, size_t skip = 2, const std::string &prefix = "    ");
}

#endif