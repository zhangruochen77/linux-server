/**
 * 日志框架 定义头文件
 */

#ifndef __SERVER_LOG_H__
#define __SERVER_LOG_H__
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream>
#include <memory>
#include <map>
#include <functional> // 函数式编程头文件
#include <cstdarg>    // va_list 头文件 va 系类头文件

#include "../utils/utils.h"
#include "../utils/singleton.h"

/**
 * @brief 流式定义打印日志宏 编译执行时生产代码自动拼接
 */
#define LOGGER(logger, level)               \
    if (logger->getLevel() <= level)        \
    server::LogEventWrap(                   \
        std::make_shared<server::LogEvent>( \
            logger,                         \
            level,                          \
            __FILE__,                       \
            __LINE__,                       \
            0,                              \
            server::GetThreadId(),          \
            server::GetFiberId(),           \
            time(0),                        \
            "main thread"))                 \
        .getSs()

/**
 * @brief debug 级别流式写入日志
 */
#define DEBUG(logger) LOGGER(logger, server::LogLevel::DEBUG)
/**
 * @brief info 级别流式写入日志
 */
#define INFO(logger) LOGGER(logger, server::LogLevel::INFO)
/**
 * @brief warn 级别流式写入日志
 */
#define WARN(logger) LOGGER(logger, server::LogLevel::WARN)
/**
 * @brief error 级别流式写入日志
 */
#define ERROR(logger) LOGGER(logger, server::LogLevel::ERROR)
/**
 * @brief fatal 级别流式写入日志
 */
#define FATAL(logger) LOGGER(logger, server::LogLevel::FATAL)

/**
 * @brief 格式化打印日志
 */
#define FMT_LOGGER(logger, level, fmt, ...)     \
    if (logger->getLevel() <= level)            \
        server::LogEventWrap(                   \
            std::make_shared<server::LogEvent>( \
                logger,                         \
                level,                          \
                __FILE__,                       \
                __LINE__,                       \
                0,                              \
                server::GetThreadId(),          \
                server::GetFiberId(),           \
                time(0),                        \
                "main thread"))                 \
            .getEvent()                         \
            ->format(fmt, __VA_ARGS__);

/**
 * @brief debug 格式化打印方式
 */
#define FMT_DEBUG(logger, fmt, ...) FMT_LOGGER(logger, server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
/**
 * @brief info 格式化打印方式
 */
#define FMT_INFO(logger, fmt, ...) FMT_LOGGER(logger, server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
/**
 * @brief warn 格式化打印方式
 */
#define FMT_WARN(logger, fmt, ...) FMT_LOGGER(logger, server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
/**
 * @brief error 格式化打印方式
 */
#define FMT_ERROR(logger, fmt, ...) FMT_LOGGER(logger, server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
/**
 * @brief fatal 格式化打印方式
 */
#define FMT_FATAL(logger, fmt, ...) FMT_LOGGER(logger, server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define SIG_LOG_ROOT() server::LogMgr::getInstance()->getRoot()

/**
 * @brief 通过名称获取日志器
 */
#define SIG_LOG_NAME(name) server::LogMgr::getInstance()->getLogger(name)

/**
 * @brief 将类与函数定义在自身的命名空间当中
 */
namespace server
{

    /**
     * @brief 预定义 防止类查找不到存在
     */
    class LogEvent;
    class Logger;
    class LogAppender;

    /**
     * 日志级别类
     */
    class LogLevel
    {
    public:
        /**
         * 日志级别枚举
         */
        enum Level
        {
            /**
             * 未知级别
             */
            UNKNOW = 0,
            /**
             * debug 级别
             */
            DEBUG = 1,
            /**
             * info 级别
             */
            INFO = 2,
            /**
             * warn 级别
             */
            WARN = 3,
            /**
             * error 级别
             */
            ERROR = 4,
            /**
             * fatal 级别
             */
            FATAL = 5
        };

        /**
         * 转换日志级别为字符串
         */
        static const std::string toString(const LogLevel::Level &level);

    private:
    };

    /**
     * 日志事件 每次需要打印的日志 按照一个事件进行打印
     */
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        /**
         * 构造器 进行日志事件信息的初始化
         */
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, std::string file,
                 uint32_t line, uint64_t elapse, uint32_t threadId,
                 uint32_t fiberId, uint64_t time, std::string threadName)
            : m_logger(logger),
              m_level(level),
              m_file(file),
              m_line(line),
              m_elapse(elapse),
              m_threadId(threadId),
              m_fiberId(fiberId),
              m_time(time),
              m_threadName(threadName) {}

        /**
         * 基本成员的 get 方法 常量修饰 保证不会被意外更改
         */
        LogLevel::Level getLevel() const { return m_level; }
        std::string getFile() const { return m_file; }
        uint32_t getLine() { return m_line; }
        uint64_t getElapse() { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        std::string getThreadName() const { return m_threadName; }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

        /**
         * stringstream 没有拷贝构造函数和赋值构造函数
         */
        std::stringstream &getSS() { return m_ss; }

        /**
         * @brief 格式化写入日志内容
         */
        void format(const char *fmt, ...);

        /**
         * @brief 格式化写入日志内容
         */
        void format(const char *fmt, va_list al);

        /**
         * 获取日志内容
         */
        std::string getContent() { return m_ss.str(); }

    private:
        std::shared_ptr<Logger> m_logger; // 所属的日志器
        LogLevel::Level m_level;          // 日志级别
        std::string m_file;               // 文件名称
        uint32_t m_line;                  // 行号
        uint64_t m_elapse;                // 程序启动毫秒数
        uint32_t m_threadId;              // 线程编号
        uint32_t m_fiberId;               // 协程编号
        uint64_t m_time;                  // 事件戳
        std::string m_threadName;         // 线程名称
        std::stringstream m_ss;           // 日志输出流
    };

    /**
     * 日志格式化器
     */
    class LogFormmtter
    {
        /**
         * 设置友元类 提供 logger 访问权限
         */
        friend class Logger;

    public:
        typedef std::shared_ptr<LogFormmtter> ptr;

        /**
         * 日志打印操作
         */
        std::ostream &format(std::ostream &os, const std::shared_ptr<Logger> &logger, LogLevel::Level level, LogEvent::ptr event);

        /**
         * 日志器
         */
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem() {}

            /**
             * 纯虚函数
             * 针对自己所能格式化的类型 与相应的参数进行格式化
             * @param os 输出流 日志打印的位置
             * @param logger 日志器
             * @param level 打印日志级别
             * @param event 日志事件 提供打印参数
             */
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

    public:
        LogFormmtter(const std::string &pattern); // 初始化函数
        ~LogFormmtter();                          // 析构函数
        void init();                              // 初始化日志格式器

    private:
        std::string m_pattern;                // 日志格式
        bool m_error;                         // 解析是否发生错误
        std::vector<FormatItem::ptr> m_items; // 存放格式化的集合
    };

    /**
     * 日志添加器
     */
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender() {}

        /**
         * 打印日志
         */
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        /**
         * 获取添加器自身的日志格式化器
         */
        LogFormmtter::ptr getFormatter() const { return m_formatter; }
        /**
         * 设置自身格式化器信息
         */
        void setFormatter(LogFormmtter::ptr ptr) { m_formatter = ptr; }

        /**
         * 获取是否具有自身格式化器标识
         */
        bool getHasSelfFormatter() const { return m_hasSelefFormatter; }

        /**
         * 设置自身格式化器标识
         */
        void setHashSelefFormatter(bool val) { m_hasSelefFormatter = val; }

    protected:
        LogLevel::Level m_level = LogLevel::Level::DEBUG; // 默认日志级别
        LogFormmtter::ptr m_formatter;                    // 日志格式化器
        bool m_hasSelefFormatter;                         // 是否有自己的默认格式化器
    };

    /**
     * 日志处理类 开启自身自身共享
     */
    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        typedef std::shared_ptr<Logger> ptr;

        /**
         * 打印普通日志
         * @param level 日志级别
         * @param event 需要打印的事件信息
         */
        void log(LogLevel::Level level, const LogEvent::ptr &event);
        /**
         * debug 日志
         */
        void debug(const LogEvent::ptr &event);
        /**
         * warn 级别
         */
        void warn(const LogEvent::ptr &event);
        /**
         * error 级别
         */
        void error(const LogEvent::ptr &event);
        /**
         * fatal 级别
         */
        void fatal(const LogEvent::ptr &event);

        /**
         * 构造器 做日志器名称初始化 默认设置名称为 root
         */
        Logger(const std::string &name = "root");

        /**
         * 销毁构造
         */
        ~Logger() {}

        /**
         * 获取格式化器
         */
        LogFormmtter::ptr getLogFormatter() const;

        /**
         * 设置日志格式化器
         */
        void setLogFormatter(LogFormmtter::ptr formatter);

        /**
         * 设置指定字符串格式 生成一个日志格式化器
         */
        void setLogFormatter(const std::string &fmt);

        /**
         * 获取日志器名称
         */
        std::string getName() const { return m_name; }

        /**
         * 添加日志输出器
         */
        void addAppender(const LogAppender::ptr &appender) { m_appenders.push_back(appender); }

        /**
         * 获取日志级别
         */
        LogLevel::Level getLevel() const { return m_level; }

    private:
        LogLevel::Level m_level = LogLevel::Level::DEBUG; // 日志级别
        std::string m_name;                               // 日志器名称
        LogFormmtter::ptr m_formatter;                    // 格式化器
        std::vector<LogAppender::ptr> m_appenders;        // 输出器 向指定的位置进行输出
    };

    /**
     * 向控制台输出的日志打印器
     */
    class StdoutLogAppender : public LogAppender
    {
    public:
        StdoutLogAppender(){};
        ~StdoutLogAppender(){};
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    /**
     * 日志管理器
     */
    class LogManager
    {
    public:
        /**
         * 默认构造 初始化信息
         */
        LogManager();
        ~LogManager() {}

        /**
         * 获取指定名称的日志器
         */
        Logger::ptr getLogger(const std::string &name) const;

        /**
         * 初始化方式
         */
        void init();

        /**
         * 获取主日志器
         */
        Logger::ptr getRoot() const { return m_root; };

    private:
        std::map<std::string, Logger::ptr> m_loggers; // 管理多个日志器 key-value pairs
        Logger::ptr m_root;                           // 主日志器
    };

    class LogEventWrap
    {
    public:
        LogEventWrap(LogEvent::ptr e) : m_event(e) {}
        ~LogEventWrap();
        LogEvent::ptr getEvent() const { return m_event; }

        /**
         * 获取日志流
         */
        std::stringstream &getSs();

    private:
        LogEvent::ptr m_event; // 存放日志事件
    };

    typedef server::Singleton<LogManager> LogMgr; // 单例的日志管理器
}

#endif