#include "server/server.h"

void test01() {
    std::cout << "fiber test01" << std::endl;
}

void test02() {
    std::cout << "fiber test02 say hello" << std::endl;
}


int main() {

    server::Scheduler::ptr sche = std::make_shared<server::Scheduler>(1, "sche");
    server::Fiber::ptr fib = std::make_shared<server::Fiber>(&test01, 4086);
    server::Scheduler::FiberAndCb fc(fib);
    server::Scheduler::FiberAndCb fc2(&test02);
    sche->schedule(fc);
    sche->schedule(fc2);
    sche->start();

    sleep(5);
    std::cout << "main over" << std::endl;
    sche->stop();
    return 0;
}