#include <iostream>
#include "log.h"

int main()
{
    server::LogFormmtter::ptr fmt = std::make_shared<server::LogFormmtter>("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    auto appender = std::make_shared<server::StdoutLogAppender>();
    server::Logger::ptr logger = std::make_shared<server::Logger>("server");
    logger->setLogFormatter(fmt);
    logger->addAppender(appender);
    LOGGER(logger, server::LogLevel::DEBUG) << "hello";
    ERROR(logger) << "error";

    FMT_ERROR(logger, "test fmt %s\n", "error");
    INFO(SIG_LOG_ROOT()) << "test single log root";
}