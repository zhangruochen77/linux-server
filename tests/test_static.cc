#include <iostream>

int MyStaticFunction()
{
    static int myStaticVariable = 0;
    myStaticVariable++; // 每次调用自增1
    return myStaticVariable;
}

int main()
{
    for (int i = 0; i < 5; i++)
    {
        int result = MyStaticFunction();
        std::cout << "Result: " << result << std::endl;
    }

    return 0;
}