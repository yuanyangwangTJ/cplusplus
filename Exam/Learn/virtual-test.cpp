#include <iostream>

class Base {
public:
    virtual ~Base() { std::cout << "Base Destructor\n"; }  // 虚析构函数
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived Destructor\n"; }  // 让基类指针正确析构
    void show() { std::cout << "Derived::show()\n"; }
};

int main() {
    Base* basePtr = new Derived();

    // static_cast 进行向下转换
    Derived* derivedPtr = static_cast<Derived*>(basePtr);
    derivedPtr->show();

    // void* 转回 Base*
    void* voidPtr = basePtr;
    Base* basePtr2 = static_cast<Base*>(voidPtr);

    // 正确的删除方式
    delete basePtr2;  // 先调用 Derived 析构函数，再调用 Base 析构函数

    return 0;
}
