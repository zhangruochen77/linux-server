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
            DEBUD = 1,
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
     * 日志格式化器
     */
    class LogFormmtter
    {
    public:
        /**
         * 日志器
        */
        class FormatItem {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){};
        };

    public:
        LogFormmtter(const std::string &pattern); // 初始化函数
        ~LogFormmtter();                          // 析构函数
        void init();                              // 初始化日志格式器

    private:
        std::string m_pattern; // 日志格式
        bool m_error; // 解析是否发生错误
        std::vector<FormatItem::ptr> m_items; // 存放格式化的集合
    };
}

#endif