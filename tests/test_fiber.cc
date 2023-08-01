#include "../server/server.h"

void fun()
{
    server::Fiber::ptr fiber = server::Fiber::GetThis();
    std::cout << "fiber start......" << std::endl;
    fiber->swapOut();
    std::cout << "fiber continue..." << std::endl;
    fiber->swapOut();
    std::cout << "fiber end........ " << std::endl;
}

void fun2()
{
    std::cout << "main start------" << std::endl;
    server::Fiber::ptr fiber = std::make_shared<server::Fiber>(&fun, 1024);
    fiber->swapIn();
    std::cout << "main continue---" << std::endl;
    fiber->swapIn();
    std::cout << "main next-------" << std::endl;
    fiber->swapIn();
    std::cout << "main end--------" << std::endl;
    server::Fiber::ptr mainFiber = server::Fiber::GetMain();
    std::cout << mainFiber.use_count() << std::endl;
    std::cout << "main over fiber count" << fiber.use_count() << std::endl;
}

int main()
{
    fun2();
    int a = 1 + 11;
    std::cout << a;
    return 0;
}