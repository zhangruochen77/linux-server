#include <iostream>

thread_local int a = 10;

int main() {
    std::cout << a << std::endl;
    return 0;
}