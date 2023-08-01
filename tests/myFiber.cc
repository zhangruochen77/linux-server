#include <iostream>
#include <ucontext.h>

ucontext_t main_ctx; // 主协程上下文
ucontext_t child_ctx; // 子协程上下文

void fun() {
    std::cout << "child start" << std::endl;
    swapcontext(&child_ctx, &main_ctx);
    std::cout << "child end" << std::endl;
}

int main() {
    std::cout << "main start" << std::endl;
    getcontext(&main_ctx); // 获取到当前协程上下文

    void * stack = malloc(1024); // 开辟协程空间
    child_ctx.uc_link = &main_ctx;
    child_ctx.uc_stack.ss_sp = stack;
    child_ctx.uc_stack.ss_size = sizeof(stack);

    getcontext(&child_ctx); // 获取子协程
    makecontext(&child_ctx, &fun, 0); // 设置子协程执行函数以及参数信息

    std::cout << "switch ctx to child" << std::endl;
    swapcontext(&main_ctx, &child_ctx); // 切换协程到子协程执行
    std::cout << "child to main" << std::endl;
    swapcontext(&main_ctx, &child_ctx);
    std::cout << "main end" << std::endl;
    return 0;
}