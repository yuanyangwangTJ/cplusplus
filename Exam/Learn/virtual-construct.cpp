#include <iostream>

class Base {
    public:
        virtual ~Base() {} // 需要虚析构函数保证基类指针删除时调用派生类析构函数
        static Base* create(); // 工厂方法
    };
    
    class Derived : public Base {
    public:
        Derived() { std::cout << "Derived constructor" << std::endl; }
    };
    
    Base* Base::create() {
        return new Derived(); // 通过工厂方法动态创建对象
    }
    
    int main() {
        Base* obj = Base::create();
        delete obj; // 触发多态析构
        return 0;
    }