/**
 * log 框架基本功能实现 cpp 文件
 */
#include "log.h"
#include "../config/config.h"

namespace server
{
    /**
     * 转换日志级别为字符串类型
     */
    const std::string LogLevel::toString(const LogLevel::Level &level)
    {
        switch (level)
        {
#define LOG_LEVEL(e)         \
    case LogLevel::Level::e: \
        return #e;           \
        break;

            LOG_LEVEL(UNKNOW);
            LOG_LEVEL(DEBUG);
            LOG_LEVEL(INFO);
            LOG_LEVEL(WARN);
            LOG_LEVEL(ERROR);
            LOG_LEVEL(FATAL);
#undef LOG_LEVEL

        default:
            return "UNKNOWN";
        }

        return "UNKNOWN";
    }

    /**
     * 从字符串中转换日志级别
     */
    LogLevel::Level LogLevel::FromString(const std::string &str)
    {
#define FROM_STRING(level, value) \
    if (#value == str)            \
    {                             \
        return level;             \
    }

        FROM_STRING(LogLevel::Level::DEBUG, debug);
        FROM_STRING(LogLevel::Level::INFO, info);
        FROM_STRING(LogLevel::Level::WARN, warn);
        FROM_STRING(LogLevel::Level::ERROR, error);
        FROM_STRING(LogLevel::Level::FATAL, fatal);

        FROM_STRING(LogLevel::Level::DEBUG, DEBUG);
        FROM_STRING(LogLevel::Level::INFO, INFO);
        FROM_STRING(LogLevel::Level::WARN, WARN);
        FROM_STRING(LogLevel::Level::ERROR, ERROR);
        FROM_STRING(LogLevel::Level::FATAL, FATAL);
#undef XX
        return LogLevel::Level::UNKNOW;
    }

    /**
     * 消息格式化器
     */
    class MessageFormatItem : public LogFormmtter::FormatItem
    {
    public:
        MessageFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getContent();
        }
    };

    /**
     * 日志级别格式化器
     */
    class LevelFormatItem : public LogFormmtter::FormatItem
    {
    public:
        LevelFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << LogLevel::toString(level);
        }
    };

    /**
     * 程序启动时间格式化器
     */
    class ElapseFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ElapseFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "start:" << event->getElapse();
        }
    };

    /**
     * 日志名称格式化器
     */
    class NameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        NameFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << logger->getName();
        }
    };

    /**
     * 线程编号格式化器
     */
    class ThreadIdFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ThreadIdFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "tId:" << event->getThreadId();
        }
    };

    /**
     * 换行格式化器
     */
    class NewLineFormatItem : public LogFormmtter::FormatItem
    {
    public:
        NewLineFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "\n";
        }
    };

    /**
     * 时间格式化器
     */
    class DateTimeFormatItem : public LogFormmtter::FormatItem
    {
    public:
        DateTimeFormatItem(const std::string fmt = "%Y-%m-%d %H:%M:%S") : m_fmt(fmt)
        {
            if (this->m_fmt.empty())
            {
                this->m_fmt = "%Y-%m-%d %H:%M:%S";
            }
        }

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            struct tm time;
            time_t t = event->getTime();
            localtime_r(&t, &time);
            char buf[64];
            strftime(buf, sizeof(buf), m_fmt.c_str(), &time);
            os << buf;
        }

    private:
        std::string m_fmt;
    };

    /**
     * 文件名称格式化器
     */
    class FilenameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        FilenameFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getFile();
        }
    };

    /**
     * 行号格式化器
     */
    class LineFormatItem : public LogFormmtter::FormatItem
    {
    public:
        LineFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getLine();
        }
    };

    /**
     * tab 格式化器
     */
    class TabFormatItem : public LogFormmtter::FormatItem
    {
    public:
        TabFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "\t";
        }
    };

    /**
     * 协程格式化器
     */
    class FiberIdFormatItem : public LogFormmtter::FormatItem
    {
    public:
        FiberIdFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "fId:" << event->getFiberId();
        }
    };

    /**
     * 线程格式化器
     */
    class ThreadNameFormatItem : public LogFormmtter::FormatItem
    {
    public:
        ThreadNameFormatItem(const std::string fmt = "") {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getThreadName();
        }
    };

    /**
     * 字符串格式化器
     */
    class StringFormatItem : public LogFormmtter::FormatItem
    {
    public:
        StringFormatItem(const std::string fmt = "") : m_message(fmt) {}

        /**
         * 纯虚函数
         * 针对自己所能格式化的类型 与相应的参数进行格式化
         * @param os 输出流 日志打印的位置
         * @param logger 日志器
         * @param level 打印日志级别
         * @param event 日志事件 提供打印参数
         */
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
        {
            os << this->m_message;
        }

    private:
        std::string m_message;
    };

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
                        fmt = m_pattern.substr(begin + 1, n - begin - 1);
                        // nstr = m_pattern.substr(begin, n - 1);
                        // std::cout << "nstr = " << nstr << std::endl;
                        ++n;
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
                    // std::cout << "nstr:" << nstr << std::endl;
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                // std::cout << "str:" << str << " fmt:" << std::endl;
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
                if (item_map.end() != it)
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
     * 构造器 做日志器名称初始化 默认设置名称为 root 打印级别为 debug 以及设置一个默认的打印器
     */
    Logger::Logger(const std::string &name)
        : m_level(LogLevel::Level::DEBUG), m_name(name)
    {
        this->m_lock = new LockType();
        this->m_formatter.reset(new LogFormmtter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }

    /**
     * 打印普通日志
     * @param level 日志级别
     * @param event 需要打印的事件信息
     */
    void Logger::log(LogLevel::Level level, const LogEvent::ptr &event)
    {
        auto self = shared_from_this();
        LockType::Lock lock(m_lock);
        if (!m_appenders.empty())
        {
            for (const auto &e : m_appenders)
            {
                e->log(self, level, event);
            }
        }
    }

    /**
     * debug 日志
     */
    void Logger::debug(const LogEvent::ptr &event)
    {
        this->log(this->m_level, event);
    }

    /**
     * warn 级别
     */
    void Logger::warn(const LogEvent::ptr &event)
    {
        this->log(this->m_level, event);
    }

    /**
     * error 级别
     */
    void Logger::error(const LogEvent::ptr &event)
    {
        this->log(this->m_level, event);
    }

    /**
     * fatal 级别
     */
    void Logger::fatal(const LogEvent::ptr &event)
    {
        this->log(this->m_level, event);
    }

    /**
     * 获取格式化器
     */
    LogFormmtter::ptr Logger::getLogFormatter() const
    {
        return this->m_formatter;
    }

    /**
     * 设置日志格式化器
     */
    void Logger::setLogFormatter(LogFormmtter::ptr formatter)
    {
        this->m_formatter = formatter;

        // 查看输出器 输出器没有自身的格式化器 没有添加格式化器
        if (!this->m_appenders.empty())
        {
            for (const auto &apd : this->m_appenders)
            {
                if (!apd->getHasSelfFormatter())
                {
                    apd->setFormatter(formatter);
                    apd->setHashSelefFormatter(true);
                }
            }
        }
    }

    /**
     * 设置指定字符串格式 生成一个日志格式化器
     */
    void Logger::setLogFormatter(const std::string &fmt)
    {
        this->m_formatter = std::make_shared<LogFormmtter>(fmt);
    }

    /**
     * 向标准输出流中打印日志
     */
    void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
    {
        logger->getLogFormatter()->format(std::cout, logger, level, event);
    }

    /**
     * 调用日志转换 返回存放进入的流数据
     */
    std::ostream &LogFormmtter::format(std::ostream &os, const std::shared_ptr<Logger> &logger, LogLevel::Level level, LogEvent::ptr event)
    {
        for (const auto &e : m_items)
        {
            e->format(os, logger, level, event);
        }

        return os;
    }

    /**
     * 进行管理器初始化操作
     */
    LogManager::LogManager()
    {
        this->init();
    }

    /**
     * 初始化 设置默认的管理器 设置 logger 默认 appender， 添加 logger 进入日志器集合进行管理
     */
    void LogManager::init()
    {
        this->m_root.reset(new Logger());                                 // 构建一个默认的 logger
        this->m_root->addAppender(std::make_shared<StdoutLogAppender>()); // 添加一个控制台 appender
        this->m_loggers[m_root->getName()] = m_root;                      // 添加 looger 进入日志器集合
    }

    /**
     * 删除日志器
     */
    void LogManager::delLogger(const std::string &name)
    {
        this->m_loggers.erase(name);
    }

    /**
     * @brief 可变参数写入日志 这里可变参数化为 va List 通过便利 list 写入参数到输出流当中
     */
    void LogEvent::format(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->format(fmt, args);
        va_end(args);
    }

    /**
     * 将可变参数数据写入到输出流中
     */
    void LogEvent::format(const char *fmt, va_list al)
    {
        char *buf = nullptr;
        int len = vasprintf(&buf, fmt, al);
        if (len != -1)
        {
            m_ss << std::string(buf, len);
            free(buf);
        }
    }

    /**
     * 调用日志进行打印操作
     * 正真的日志打印 是在这个对象结束的时候 进行的打印实现
     * 通过其销毁函数 前面是对 event 需要打印的数据进行解析 以及所需要的数据进行构造
     */
    LogEventWrap::~LogEventWrap()
    {
        this->m_event->getLogger()->log(this->m_event->getLevel(), this->m_event);
    }

    /**
     * 返回构建的日志流 用于插入后续数据 打印用户的日志信息
     */
    std::stringstream &LogEventWrap::getSs()
    {
        return this->m_event->getSS();
    }

    /**
     * 初始化 负责文件的打开
     */
    FileLogAppender::FileLogAppender(const std::string &filename)
    {
        // 打开文件，转化为流 文件打开失败 直接抛出异常
        this->m_ofstream.open(filename, std::ios::app | std::ios::out);
        this->m_filename = filename;
        this->m_lastTime = time(0);
    }

    /**
     * 销毁，处理关闭文件流
     */
    FileLogAppender::~FileLogAppender()
    {
        this->m_ofstream.close();
    }

    /**
     * 打印日志
     */
    void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
    {
        logger->getLogFormatter()->format(this->m_ofstream, logger, level, event);
    }

    /**
     * @brief 重新打开文件流
     */
    bool FileLogAppender::reopen()
    {
        this->m_ofstream.close();
        this->m_ofstream.open(this->m_filename, std::ios::app | std::ios::out);

        return this->m_ofstream.is_open();
    }

    /**
     * 转换 yaml 格式
     */
    std::string LogManager::toYamlString(Logger::ptr logger)
    {
        return logger->toYamlString();
    }

    /**
     * 通过名称获取 logger
     */
    Logger::ptr LogManager::getLogger(const std::string &name)
    {
        auto pair = m_loggers.find(name);
        if (m_loggers.end() != pair)
        {
            return pair->second;
        }

        Logger::ptr ptr = std::make_shared<Logger>(name);
        m_loggers[name] = ptr;

        return ptr;
    }

    /**
     * 转换为 yaml 格式
     */
    std::string Logger::toYamlString()
    {
        std::stringstream ss;
        YAML::Node node;
        node["name"] = this->m_name;
        node["level"] = LogLevel::toString(this->m_level);
        node["formatter"] = this->m_formatter->m_pattern;
        for (auto &i : this->m_appenders)
        {
            node["appenders"].push_back(YAML::Load(i->toYamlString()));
        }

        ss << node;
        return ss.str();
    }

    /**
     * 转换日志格式 yaml
     */
    std::string StdoutLogAppender ::toYamlString()
    {
        std::stringstream ss;
        YAML::Node node;
        node["type"] = "StdoutLogAppender";
        ss << node;
        return ss.str();
    }

    /**
     * 转换日志格式 yaml
     */
    std::string FileLogAppender::toYamlString()
    {
        std::stringstream ss;
        YAML::Node node;
        node["type"] = "FileLogAppender";
        node["file"] = this->m_filename;
        ss << node;
        return ss.str();
    }

    /**
     * 定义日志输出器 yaml 对象格式
     */
    struct LogAppenderDefine
    {
        int type = 0;          // file -> 1      stdout -> 2
        std::string file;      // 文件名称
        std::string formatter; // 格式化模板

        bool operator==(const LogAppenderDefine &lad) const
        {
            return type == lad.type && file == lad.file && formatter == lad.formatter;
        }
    };

    /**
     * 定义日志对象 yaml 格式
     */
    struct LogDefine
    {
        std::string name;                         // 日志名称
        LogLevel::Level level;                    // 日志级别
        std::vector<LogAppenderDefine> appenders; // 日志输出器集合
        std::string formatter;                    // 日志格式化模板

        bool operator==(const LogDefine &ld) const
        {
            return name == ld.name && level == ld.level && appenders == ld.appenders && formatter == ld.formatter;
        }

        bool operator<(const LogDefine &ld) const
        {
            return name < ld.name;
        }
    };

    /**
     * yaml 格式转 logger 偏特化
     */
    template <>
    class LexicalCast<std::string, LogDefine>
    {
    public:
        LogDefine operator()(const std::string &val)
        {
            LogDefine ld;                      // 偏特化返回结果
            YAML::Node node = YAML::Load(val); // 加载值转换为 node 节点

            // 解析日志名称
            if (!node["name"].IsDefined())
            {
                std::cout << "can't find root name in application" << std::endl;
                throw std::logic_error("can't find root name in application");
            }
            else
            {
                ld.name = node["name"].as<std::string>();
            }

            // 解析日志级别
            if (node["level"].IsDefined())
            {
                ld.level = LogLevel::FromString(node["level"].as<std::string>());
            }
            else
            {
                ld.level = LogLevel::Level::UNKNOW;
            }

            // 解析格式化器
            if (!node["formatter"].IsDefined())
            {
                std::cout << "can't find formatter in application to " << ld.name << std::endl;
                throw std::logic_error(std::string("can't find formatter in application to ") + ld.name);
            }
            else
            {
                ld.formatter = node["formatter"].as<std::string>();
            }

            // 解析日志输出器
            if (!node["appenders"].IsDefined())
            {
                std::cout << "can't find appenders in application to " << ld.name << std::endl;
                throw std::logic_error(std::string("can't find appenders in application to ") + ld.name);
            }
            else
            {
                for (size_t i = 0; i < node["appenders"].size(); ++i)
                {
                    YAML::Node a = node["appenders"][i];
                    LogAppenderDefine lad;
                    if (!a["type"].IsDefined())
                    {
                        std::cout << "can't find appenders type in application to " << ld.name << std::endl;
                        throw std::logic_error(std::string("can't find appenders type in application to ") + ld.name);
                    }
                    else if ("FileLogAppender" == a["type"].as<std::string>())
                    {
                        lad.type = 1;
                        // 文件信息解析
                        if (!a["file"].IsDefined())
                        {
                            std::cout << "can't find appenders file in application to " << ld.name << std::endl;
                            throw std::logic_error(std::string("can't find appenders file in application to ") + ld.name);
                        }
                        else
                        {
                            lad.file = a["file"].as<std::string>();
                        }
                    }
                    else if ("StdOutLogAppender" == a["type"].as<std::string>())
                    {
                        lad.type = 2;
                    }
                    else
                    {
                        std::cout << "unknow appenders type in application to " << ld.name << std::endl;
                        throw std::logic_error(std::string("unknow") + a["type"].as<std::string>() + std::string("appenders type in application to ") + ld.name);
                    }

                    // 查看日志输出器是否有自己设定的格式化模板
                    if (a["formatter"].IsDefined())
                    {
                        lad.formatter = a["formatter"].as<std::string>();
                    }

                    ld.appenders.push_back(lad);
                }
            }

            return ld;
        }
    };

    /**
     * @brief 日志到 yaml 格式偏特化
     */
    template <>
    class LexicalCast<LogDefine, std::string>
    {
    public:
        std::string operator()(const LogDefine &ld)
        {
            std::stringstream ss;
            YAML::Node node;
            node["name"] = ld.name;
            node["level"] = LogLevel::toString(ld.level);
            node["formatter"] = ld.formatter;
            if (!ld.appenders.empty())
            {
                for (auto lad : ld.appenders)
                {
                    YAML::Node n;
                    if (1 == lad.type)
                    {
                        n["type"] = "FileLogAppender";
                        n["file"] = lad.file;
                    }
                    else if (2 == lad.type)
                    {
                        n["type"] = "StdOutLogAppender";
                    }

                    n["formatter"] = lad.formatter;
                    node["appenders"].push_back(n);
                }
            }

            return ss.str();
        }
    };

    server::ConfigVar<std::set<LogDefine>>::ptr g_log_defines = server::Config::Lookup("application", std::set<LogDefine>(), "logs config");

    /**
     * looger 的全局初始化方式 进行回调函数的设置 对logger进行更新操作
     */
    struct LogIniter
    {
        LogIniter()
        {
            g_log_defines->addListener(
                [](const std::set<LogDefine> &oldVal, const std::set<LogDefine> &newVal)
                {
                    for (auto &ld : newVal)
                    {
                        Logger::ptr logger;

                        // 添加 old 没有 new 有
                        if (oldVal.end() == oldVal.find(ld))
                        {
                            logger = SIG_LOG_NAME(ld.name);
                        }
                        // 更新 old 有 new 有 更新字段
                        else
                        {
                            if (ld == *(oldVal.find(ld)))
                            {
                                continue;
                            }

                            logger = SIG_LOG_NAME(ld.name);
                        }

                        if (!ld.formatter.empty())
                        {
                            logger->setLogFormatter(ld.formatter);
                        }

                        logger->setLevel(ld.level);
                        if (!ld.appenders.empty())
                        {
                            logger->clearAppenders();
                            for (auto &lad : ld.appenders)
                            {
                                LogAppender::ptr apd;
                                if (1 == lad.type)
                                {
                                    apd.reset(new FileLogAppender(lad.file));
                                }
                                else if (2 == lad.type)
                                {
                                    apd.reset(new StdoutLogAppender());
                                }

                                if (!lad.formatter.empty())
                                {
                                    apd->setFormatter(std::make_shared<LogFormmtter>(lad.formatter));
                                }

                                apd->setLevel(ld.level);
                                logger->addAppender(apd);
                            }
                        }
                    }

                    // 删除 old 有 new 没有 删除
                    for (auto &ld : oldVal)
                    {
                        if (newVal.end() != newVal.find(ld))
                        {
                            Logger::ptr logger = SIG_LOG_NAME(ld.name);
                            logger->clearAppenders();
                            LogMgr::getInstance()->delLogger(ld.name);
                        }
                    }
                });
        }
    };

    /**
     * 进行初始化操作 设置回调函数 构建静态变量 优先调用构造函数 进行logger回调函数的设置
     */
    static server::LogIniter __log_init;
}
