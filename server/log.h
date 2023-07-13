/**
 * 日志框架 定义头文件
 */

#ifndef __SERVER_LOG_H__
#define __SERVER_LOG_H__
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <memory>
#include <map>
#include <functional>

/**
 * 将类与函数定义在自身的命名空间当中
 */
namespace server
{

    /**
     * 预定义 防止类查找不到存在
     */
    class LogEvent;
    class Logger;

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
        LogEvent(LogLevel::Level level, std::string file,
                 uint32_t line, uint64_t elapse, uint32_t threadId,
                 uint32_t fiberId, uint64_t time, std::string threadName)
            : m_level(level),
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

    private:
        LogLevel::Level m_level;  // 日志级别
        std::string m_file;       // 文件名称
        uint32_t m_line;          // 行号
        uint64_t m_elapse;        // 程序启动毫秒数
        uint32_t m_threadId;      // 线程编号
        uint32_t m_fiberId;       // 协程编号
        uint64_t m_time;          // 事件戳
        std::string m_threadName; // 线程名称
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
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

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
        void log(LogLevel::Level level, LogEvent::ptr event);
        /**
         * debug 日志
         */
        void debug(LogEvent::ptr event);
        /**
         * warn 级别
         */
        void warn(LogEvent::ptr event);
        /**
         * error 级别
         */
        void error(LogEvent::ptr event);
        /**
         * fatal 级别
         */
        void fatal(LogEvent::ptr event);

        /**
         * 构造器 做日志器名称初始化
         */
        Logger(const std::string &name) : m_name(name) {}

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

    private:
        LogLevel::Level m_level = LogLevel::Level::DEBUG; // 日志级别
        std::string m_name;                               // 日志器名称
        LogFormmtter::ptr m_formatter;                    // 格式化器
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

    protected:
        LogLevel::Level m_level = LogLevel::Level::DEBUG; // 默认日志级别
        LogFormmtter::ptr m_formatter;                    // 日志格式化器
        bool m_hasSelefFormatter;                         // 是否有自己的默认格式化器
    };

    /**
     * 向控制台输出的日志打印器
     */
    class StdoutLogAppender : public LogAppender
    {
    public:
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    };
}

#endif