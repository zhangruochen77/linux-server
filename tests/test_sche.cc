#include "server/server.h"

void test01() {
    std::cout << "fiber test01" << std::endl;
}

void test02() {
    std::cout << "fiber test02 say hello" << std::endl;
}


int main() {

    server::Scheduler* sche = new server::Scheduler(1, "sche");
    server::Scheduler::FiberAndCb fc(std::make_shared<server::Fiber>(&test01));
    server::Scheduler::FiberAndCb fc2(&test02);
    sche->schedule(fc);
    sche->schedule(fc2);
    sche->start();

    sleep(1000);
    sche->stop();
    return 0;
}