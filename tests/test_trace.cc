#include "server/server.h"

void fun1() {
    std::cout << server::BackTraceToString(10, 0, "    ");
}

int main() {
    fun1();

    return 0;
}