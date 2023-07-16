#include <iostream>
#include <algorithm>
#include <string>

int main() {
    std::string str = "Hello, World!";
    
    std::transform(str.begin(), str.end(), (int) 0, ::tolower);
    
    std::cout << "Lowercased string: " << str << std::endl;
    
    return 0;
}