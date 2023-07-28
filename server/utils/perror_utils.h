#ifndef __PERROR_UTILS_H__
#define __PERROR_UTILS_H__

#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstring>

namespace
{
    /**
     * 错误信息输出
     */
    void err(const char *msg)
    {
        fprintf(stderr, "==> error by %s and errorno msg %s\n", msg, strerror(errno));
    }
}

#endif