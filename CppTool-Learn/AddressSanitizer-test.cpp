// g++ -fsanitize=address -g -O1 main.cpp -o main
// ./main
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};

    std::cout << "v[2] = " << v[2] << std::endl; // ✅ 正常访问
    std::cout << "v[3] = " << v[3] << std::endl; // ❌ 越界访问

    return 0;
}