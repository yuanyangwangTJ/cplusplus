#include <cstdlib>
#include <iostream>
#include <iterator>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {}  // 确保有虚函数，支持 dynamic_cast
};

class Derived : public Base {
public:
    void show() { std::cout << "Derived class\n"; }
};

struct Data {
    int a;
    double b;
};

void modifyConst(const int* p) {
    int* modifiable = const_cast<int*>(p);
    *modifiable = 42;  // 只能对原本是非 const 的变量使用，若 p 指向真正的常量则行为未定义
}

int main() {
    std::cout << "===== static_cast Example =====\n";
    double d = 3.14;
    int i = static_cast<int>(d);  // double -> int
    std::cout << "Double to int: " << i << std::endl;

    Base* basePtr = new Derived();
    Derived* derivedPtr = static_cast<Derived*>(basePtr);  // 父类指针 -> 子类指针
    derivedPtr->show();

    void* voidPtr = basePtr;
    Base* basePtr2 = static_cast<Base*>(voidPtr);  // void* -> Base*
    basePtr2->~Base();  // 调用析构函数，防止内存泄漏
    delete basePtr;

    std::cout << "static cast void test\n";
    Data* pdata = static_cast<Data*>(malloc(sizeof(Data)));
    free(pdata);

    std::cout << "\n===== dynamic_cast Example =====\n";
    Base* b = new Derived();
    Derived* dptr = dynamic_cast<Derived*>(b);  // 安全转换，父类指针 -> 子类指针
    if (dptr) {
        std::cout << "dynamic_cast success!\n";
        dptr->show();
    } else {
        std::cout << "dynamic_cast failed!\n";
    }

    Base* b2 = new Base();
    Derived* dptr2 = dynamic_cast<Derived*>(b2);  // 失败，返回 nullptr
    if (dptr2) {
        std::cout << "Unexpected success!\n";
    } else {
        std::cout << "dynamic_cast failed as expected!\n";
    }
    delete b;
    delete b2;

    std::cout << "\n===== const_cast Example =====\n";
    int x = 10;
    const int* ptr = &x;
    std::cout << "Before modification: " << x << std::endl;
    modifyConst(ptr);
    std::cout << "After modification: " << x << std::endl;

    std::cout << "\n===== reinterpret_cast Example =====\n";
    int num = 65;
    char* c = reinterpret_cast<char*>(&num);  // 解释为 char 指针
    std::cout << "reinterpret_cast<int -> char>: " << *c << std::endl;  // 可能输出 'A' (ASCII 65)

    Data data = {42, 3.14};
    int* p = reinterpret_cast<int*>(&data);  // 结构体地址转换
    std::cout << "reinterpret_cast<Data -> int>: " << *p << std::endl;  // 可能输出 42

    return 0;
}
