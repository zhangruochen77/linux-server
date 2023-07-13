#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<int> p = std::make_shared<int>(10);
    std::cout << *p << std::endl;

    return 0;
}