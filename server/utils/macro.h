#ifndef __MACRO_H__
#define __MACRO_H__

#include <string>
#include <assert.h>
#include "utils.h"
#include "../log/log.h"

// 断言不携带消息宏
#define ASSERT(x)                                         \
    if (!x)                                               \
    {                                                     \
        ERROR(SIG_LOG_ROOT())                             \
            << std::endl                                  \
            << server::BackTraceToString(100, 0, "    "); \
        assert(x);                                        \
    }

#define ASSERT_MSG(x, msg)                                \
    if (!x)                                               \
    {                                                     \
        ERROR(SIG_LOG_ROOT())                             \
            << std::endl                                  \
            << "message: "                                \
            << #msg                                       \
            << std::endl                                  \
            << server::BackTraceToString(100, 0, "    "); \
        assert(x);                                        \
    }

#endif