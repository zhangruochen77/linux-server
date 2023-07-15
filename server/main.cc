#include <iostream>
#include "log.h"

int main()
{
    server::LogFormmtter::ptr fmt = std::make_shared<server::LogFormmtter>("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    auto appender = std::make_shared<server::StdoutLogAppender>();
    server::Logger::ptr logger = std::make_shared<server::Logger>("server");
    logger->setLogFormatter(fmt);
    logger->addAppender(appender);

    // server::LogEvent::ptr event = std::make_shared<server::LogEvent>(server::LogLevel::WARN, "file", 1, 1, 1, 1, 1, "threadName");
    // logger->log(server::LogLevel::DEBUG, event);

    // server::LogEventWrap(server::LogEvent::ptr(new server::LogEvent(logger, server::LogLevel::DEBUG,                                            \
    //         __FILE__, __LINE__, 0, 1,
    //         1, time(0), "hello"))).getSs() << "hello\n";

    LOGGER(logger, server::LogLevel::DEBUG) << "hello";
}