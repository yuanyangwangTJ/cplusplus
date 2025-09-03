#include <iostream>

class Base {
public:
    virtual void show(int x = 10) {
        std::cout << "Base::show(), x = " << x << std::endl;
    }
    virtual ~Base() {}
};

class Derived : public Base {
public:
    void show(int x = 20) override {
        std::cout << "Derived::show(), x = " << x << std::endl;
    }
};

int main() {
    Base* ptr = new Derived();
    ptr->show(); // 输出什么？
    delete ptr;
    return 0;
}