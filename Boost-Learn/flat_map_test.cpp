#include <iostream>
#include <string>
#include <boost/container/flat_map.hpp> // 包含 flat_map 头文件

int main() {
    // 声明一个 flat_map
    boost::container::flat_map<int, std::string> my_flat_map;

    // 插入元素
    my_flat_map.insert({3, "apple"});
    my_flat_map.insert({1, "banana"});
    my_flat_map.insert({2, "cherry"});
    my_flat_map.insert({4, "date"});

    std::cout << "Elements in flat_map:" << std::endl;
    for (const auto& pair : my_flat_map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 访问元素
    if (my_flat_map.count(2)) {
        std::cout << "\nValue for key 2: " << my_flat_map.at(2) << std::endl;
    }

    // flat_map 的有序性 (基于键)
    // 尽管插入顺序是 3, 1, 2, 4，但迭代时会按键的顺序排列：1, 2, 3, 4
    std::cout << "\nIterating in key order:" << std::endl;
    for (const auto& pair : my_flat_map) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;

    return 0;
}