#include <iostream>
#include <span>
#include <vector>

void modify(std::span<int> sp) {
    for (int& x : sp) {
        x *= 2;  // 修改原始数据
    }
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    std::span<int> sp(v);  // 绑定 vector
    modify(sp);  // 修改元素
    
    for (int x : v) {  // 输出原数组
        std::cout << x << " ";  // 输出: 2 4 6 8 10
    }
    std::cout << std::endl;

    return 0;
}
