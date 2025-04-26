#include <iostream>

int main() {
    int x = 10;

    // 捕获 x 的副本，并修改副本的值
    auto lambda = [x]() mutable {
        x = 20;  // 修改捕获的副本
        std::cout << "Inside lambda: x = " << x << std::endl;
    };

    std::cout << "Before lambda: x = " << x << std::endl;
    lambda();  // 调用 lambda
    std::cout << "After lambda: x = " << x << std::endl;

    return 0;
}
