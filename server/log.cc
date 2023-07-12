/**
 * log 框架基本功能实现 cpp 文件
 */
#include "log.h"

namespace server
{

    /**
     * 初始化日志格式器 为解析格式设定初始值
     */
    LogFormmtter::LogFormmtter(const std::string &pattern) : m_pattern(pattern)
    {
        this->init(); // 进行初始化操作
    }

    /**
     * 销毁初始化日志格式器
     */
    LogFormmtter::~LogFormmtter() {}

    /**
     * 初始化日志器操作 对目标格式进行格式解析
     */
    void LogFormmtter::init()
    {
        // 存放解析格式后对应关系
        std::vector<std::tuple<std::string, std::string, int>> vec;
        std::string nstr;
        int len = this->m_pattern.size();
        for (int i = 0; i < len; ++i)
        {
            // 如果不是 % 那么不断向后遍历 并且记录该字符串 这是用于处理的格式 %D
            if ('%' != m_pattern[i])
            {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            // 如果遇到 % 判断后面部分是否也为 % ， 这表明需要做转义
            if (i + 1 < len && '%' == m_pattern[i + 1])
            {
                nstr.append(1, '%');
                i++;
                continue;
            }

            // 解析格式 是否包含 {} 这种整体输出类型
            int n = i + 1;
            std::string str;
            std::string fmt;
            int begin = 0;  // 记录开始位置
            int status = 0; // 表示没有匹配 { 情况

            while (n < len)
            {
                // 非匹配 { 和 } 情况结束 没有匹配到 { 和 } 那么分割出一个解析字符串
                if (!status && (!isalpha(m_pattern[n]) && '{' != m_pattern[n] && '}' != m_pattern[n]))
                {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    // std::cout << "str = " << str << std::endl;
                    break;
                }

                // 匹配了左 { 括号
                if (!status && '{' == m_pattern[n])
                {
                    status = 1; // 设置表明遇到了 { 情况
                    str = m_pattern.substr(i + 1, n - i - 1);
                    // std::cout << "mattern { : str=" << str << std::endl;
                    begin = n;
                    ++n;
                    continue;
                }
                if (status)
                {
                    // 到达结尾
                    if ('}' == m_pattern[n])
                    {
                        status = 0;
                        // str = m_pattern.substr(i + 1, begin - i - 1);
                        fmt = m_pattern.substr(i + 1, begin - i - 1);
                        nstr = m_pattern.substr(begin, n - 1);
                        // std::cout << "nstr = " << nstr << std::endl;
                        break;
                    }
                }
                ++n;

                // 表明越界 处理最后一个格式化字符
                if (n == len)
                {
                    if (str.empty())
                    {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }

            // 将解析格式放入元组 按照替换值, 格式化值 类型存放
            if (0 == status)
            {
                // 正常解析了格式化字符串
                if (!nstr.empty())
                {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    std::cout << "nstr:" << nstr << std::endl;
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                std::cout << "str:" << str << " fmt:" << std::endl;
            }
            else
            {
                // 非正常解析格式化字符串 字符串不符合要求
                this->m_error = true;
                std::cout << "error to resolve the mattern" << std::endl;
                vec.push_back(std::make_tuple("<error_mattern>", fmt, 0));
            }

            // std::cout << "over str:" << str << " nstr:" << nstr << std::endl;
            nstr.clear(); // 使用完毕 清空字符串
            i = n - 1;
        }

        // 构建 map 集合 进行解析的类型转换
        static std::map<std::string, std::function<LogFormmtter::FormatItem::ptr(const std::string &str)>> item_map = {
#define XX(key, C)                                                                             \
    {                                                                                          \
        #key, [](const std::string &fmt) { return LogFormmtter::FormatItem::ptr(new C(fmt)); } \
    }

            XX(m, MessageFormatItem),
            XX(p, LevelFormatItem),
            XX(r, ElapseFormatItem),
            XX(c, NameFormatItem),
            XX(t, ThreadIdFormatItem),
            XX(n, NewLineFormatItem),
            XX(d, DateTimeFormatItem),
            XX(f, FilenameFormatItem),
            XX(l, LineFormatItem),
            XX(T, TabFormatItem),
            XX(F, FiberIdFormatItem),
            XX(N, ThreadNameFormatItem),
#undef XX
        };

        // 将解析后的格式化器放入集合中按序存放
        for (auto v : vec)
        {
            int i = std::get<2>(v);
            // 普通字符串 直接添加
            if (0 == i)
            {
                m_items.push_back(LogFormmtter::FormatItem::ptr(new StringFormatItem(std::get<0>(v))));
            }
            else
            {
                // 查看是否有满足的格式化器 有满足的格式化器 添加格式化器 否则添加字符串格式化器
                auto it = item_map.find(std::get<0>(v));
                if (item_map.end() == it)
                {
                    // 函数回调 创建实例对象添加入集合
                    m_items.push_back(it->second(std::get<1>(v)));
                }
                else
                {
                    m_items.push_back(LogFormmtter::FormatItem::ptr(new StringFormatItem(std::get<0>(v))));
                }
            }
        }
    }

    /**
     * 消息格式化器
     */
    class MessageFormatItem : public LogFormmtter::FormatItem
    {
    public:
        MessageFormatItem(const std::string fmt = "") {}
    };

    /**
     * 日志级别格式化器
     */
    class LevelFormatItem : public LogFormmtter::FormatItem
    {
    public:
        LevelFormatItem(const std::string fmt = "") {}
    };

    /**
     *
     */
    class ElapseFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ElapseFormatItem(const std::string fmt = "") {}
    };

    /**
     * 日志名称格式化器
     */
    class NameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        NameFormatItem(const std::string fmt = "") {}
    };

    /**
     * 线程编号格式化器
     */
    class ThreadIdFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ThreadIdFormatItem(const std::string fmt = "") {}
    };

    /**
     * 换行格式化器
     */
    class NewLineFormatItem : public LogFormmtter::FormatItem
    {
    public:
        NewLineFormatItem(const std::string fmt = "") {}
    };

    /**
     * 时间格式化器
     */
    class DateTimeFormatItem : public LogFormmtter::FormatItem
    {
    public:
        DateTimeFormatItem(const std::string fmt = "") {}
    };

    /**
     * 文件名称格式化器
     */
    class FilenameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        FilenameFormatItem(const std::string fmt = "") {}
    };

    /**
     * 行号格式化器
     */
    class LineFormatItem : public LogFormmtter::FormatItem
    {
    public:
        LineFormatItem(const std::string fmt = "") {}
    };

    /**
     * tab 格式化器
     */
    class TabFormatItem : public LogFormmtter::FormatItem
    {
    public:
        TabFormatItem(const std::string fmt = "") {}
    };

    /**
     * 协程格式化器
     */
    class FiberIdFormatItem : public LogFormmtter::FormatItem
    {
    public:
        FiberIdFormatItem(const std::string fmt = "") {}
    };

    /**
     * 线程格式化器
     */
    class ThreadNameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ThreadNameFormatItem(const std::string fmt = "") {}
    };

    /**
     * 字符串格式化器
     */
    class StringFormatItem : public LogFormmtter::FormatItem
    {
    public:
        StringFormatItem(const std::string fmt = "") {}
    };

    /**
     * 打印普通日志
     * @param level 日志级别
     * @param event 需要打印的事件信息
     */
    void Logger::log(LogLevel::Level level, LogEvent event)
    {
    }

    /**
     * debug 日志
     */
    void Logger::debug(LogEvent event)
    {
    }

    /**
     * warn 级别
     */
    void Logger::warn(LogEvent event)
    {
    }

    /**
     * error 级别
     */
    void Logger::error(LogEvent event)
    {
    }

    /**
     * fatal 级别
     */
    void Logger::fatal(LogEvent event)
    {
    }

    /**
     * 获取格式化器
     */
    LogFormmtter::ptr Logger::getLogFormatter()
    {
    }

    /**
     * 设置日志格式化器
     */
    void Logger::setLogFormatter(LogFormmtter::ptr formatter)
    {
    }

    /**
     * 设置指定字符串格式 生成一个日志格式化器
     */
    void Logger::setLogFormatter(const std::string &fmt)
    {
    }

}
