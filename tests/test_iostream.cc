#include <iostream>
#include <sstream>
#include <string>

int main() {
     std::stringstream ss;
    
    ss << "Hello, World!"; // 向 stringstream 写入内容
    
    std::cout << "Before clear - stringstream: " << ss.str() << std::endl;
    
    ss.clear(); // 清空 stringstream 的状态标志
    
    std::cout << "After clear - stringstream: " << ss.str() << std::endl;
    
    ss.str(""); // 重置 stringstream 的内容为空字符串
    
    std::cout << "After str() - stringstream: " << ss.str() << std::endl;
    
    return 0;
}