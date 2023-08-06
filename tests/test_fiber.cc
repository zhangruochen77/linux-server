#include "../server/server.h"

void fun()
{
    server::Fiber* fiber = server::Fiber::GetThis();
    std::cout << "fiber start......" << std::endl;
    fiber->swapOut();
    std::cout << "fiber continue..." << std::endl;
    fiber->swapOut();
    std::cout << "fiber end........ " << std::endl;
}

void fun2()
{
    std::cout << "main start------" << std::endl;
    server::Fiber fiber = server::Fiber(&fun, 1024);
    fiber.swapIn();
    std::cout << "main continue---" << std::endl;
    fiber.swapIn();
    std::cout << "main next-------" << std::endl;
    std::cout << "main end--------" << std::endl;
    server::Fiber::SetMain(nullptr);
}

int main()
{
    fun2();
    int a = 1 + 11;
    std::cout << a << std::endl;
    return 0;
}