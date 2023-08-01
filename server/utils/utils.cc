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

    /**
     * @brief 指针转换字符串并且释放内存
     * @param str 需要转换字符串的指针
     */
    std::string CharPointToString(char *str)
    {
        size_t size = 0;
        int status = 0;
        std::string rt;
        rt.resize(256);
        if (1 == sscanf(str, "%*[^(]%*[^_]%255[^)+]", &rt[0]))
        {
            char *v = abi::__cxa_demangle(&rt[0], nullptr, &size, &status);
            if (v)
            {
                std::string result(v);
                free(v);
                return result;
            }
        }
        if (1 == sscanf(str, "%255s", &rt[0]))
        {
            return rt;
        }
        return str;
    }

    /**
     * @brief 获取堆栈信息
     * @param[out] traces 存放堆栈信息集合
     * @param size 单次存放堆栈层级数量
     * @param skip 跳过展示层数
     */
    void BackTrace(std::vector<std::string> &traces, size_t size, size_t skip)
    {
        size_t level; // 获取到的层数
        void **buffer = (void **)malloc((sizeof(void *) * size));
        char **strings;
        level = ::backtrace(buffer, size);

        strings = ::backtrace_symbols(buffer, level);
        if (nullptr == strings)
        {
            // TODO error
        }

        for (size_t i = skip; i < level; ++i)
        {
            traces.push_back(CharPointToString(strings[i]));
        }

        for (size_t i = 0; i < skip; ++i)
        {
            if (nullptr != strings[i])
            {
                ::free(strings[i]);
            }
        }

        ::free(strings);
        ::free(buffer);
    }

    /**
     * @brief 获取堆栈信息以字符串形式
     * @param size 堆栈层数大小
     * @param skip 跳过展示层数
     * @param prefix 设置打印展示前缀
     */
    std::string BackTraceToString(size_t size, size_t skip, const std::string &prefix)
    {
        std::vector<std::string> traces;
        BackTrace(traces, size, skip);
        if (traces.empty())
        {
            return std::string();
        }

        std::stringstream ss;
        for (auto &e : traces)
        {
            ss << prefix << e << std::endl;
        }

        return ss.str();
    }
}