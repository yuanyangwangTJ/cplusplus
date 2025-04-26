#include <iostream>
#include <type_traits>

// **压缩对（Compressed Pair）**
// 该结构体用于优化存储空间，特别是当 T1 是空类时
template<typename T1, typename T2, bool = std::is_empty_v<T1>>
struct compressed_pair : T1 {  // 继承 T1 以利用 EBO（空基类优化）
    T2 val2; // 直接存储 T2 的值
    compressed_pair(const T1& t1, const T2& t2) : T1(t1), val2(t2) {} 
    T1& first() { return *this; }  // 由于 T1 是基类，我们可以用 *this 访问它
    T2& second() { return val2; }
};

// **偏特化版本：当 T1 不是空类时**
template<typename T1, typename T2>
struct compressed_pair<T1, T2, false> {
    T1 val1;  // 直接存储 T1
    T2 val2;  // 直接存储 T2
    compressed_pair(const T1& t1, const T2& t2) : val1(t1), val2(t2) {}
    T1& first() { return val1; }
    T2& second() { return val2; }
};

// **示例空类**
struct Empty {};

// **示例非空类**
struct NonEmpty {
    int x;
    NonEmpty(int val = 0) : x(val) {}  // 构造函数
};

int main() {
    // **情况 1：T1 是空类，T2 是 int**
    compressed_pair<Empty, int> obj1(Empty{}, 42);
    
    // **情况 2：T1 是非空类，T2 是 int**
    compressed_pair<NonEmpty, int> obj2(NonEmpty{10}, 42);

    // **输出 sizeof 结果**
    std::cout << "Size of Empty: " << sizeof(Empty) << std::endl;
    std::cout << "Size of int: " << sizeof(int) << std::endl;
    std::cout << "Size of NonEmpty: " << sizeof(NonEmpty) << std::endl;
    std::cout << "Size of compressed_pair<Empty, int>: " << sizeof(obj1) << std::endl;
    std::cout << "Size of compressed_pair<NonEmpty, int>: " << sizeof(obj2) << std::endl;

    // **访问元素**
    std::cout << "obj1.second (T2 val2): " << obj1.second() << std::endl;
    std::cout << "obj2.first (T1 val1.x): " << obj2.first().x << std::endl;
    std::cout << "obj2.second (T2 val2): " << obj2.second() << std::endl;

    return 0;
}
