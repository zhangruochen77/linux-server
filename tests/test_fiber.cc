#include "../server/server.h"

void fun()
{
    server::Fiber::ptr fiber = server::Fiber::GetThis();
    std::cout << "fiber start......" << fiber.use_count() << std::endl;
    fiber->swapOut();
    std::cout << "fiber continue..." << fiber.use_count() << std::endl;
    fiber->swapOut();
    std::cout << "fiber end........" << fiber.use_count() << std::endl;
    fiber = nullptr;
}

void fun2()
{

    server::Fiber::ptr fiber = std::make_shared<server::Fiber>(&fun, 10240);
    std::cout << "main start------" << fiber.use_count() << std::endl;
    fiber->swapIn();
    std::cout << "main continue---" << fiber.use_count() << std::endl;
    fiber->swapIn();
    std::cout << "main last-------" << fiber.use_count() << std::endl;
    fiber->swapIn();
    std::cout << "main end--------" << fiber.use_count() << std::endl;
}

int main()
{
    fun2();
    return 0;
}