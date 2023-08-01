#include <memory>
#include <iostream>

std::shared_ptr<int> x = nullptr;

void fun(std::shared_ptr<int> &p) {
    x = p;
}

int main() {
    std::shared_ptr<int> p = std::shared_ptr<int>(new int(1));
    std::cout << p.use_count() << std::endl;
    fun(p);
    std::cout << p.use_count() << std::endl;
}