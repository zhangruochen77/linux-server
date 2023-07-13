#include <iostream>
#include "log.h"

int main()
{
    server::LogFormmtter fmt("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");

    server::Logger logger("server");

    logger.log(server::LogLevel::Level::DEBUG,
               std::make_shared<server::LogEvent>(
                   server::LogEvent(server::LogLevel::Level::DEBUG, "file", 1, 1, 1, 1, 1, "threadName")));
}