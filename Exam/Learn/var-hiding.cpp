#include <iostream>
#include <string>

// 基类
class Base {
protected:
    int value;
    std::string name;
    static constexpr int count = 10;

public:
    Base(int v, const std::string& n) : value(v), name(n) {}
    
    void display() {
        std::cout << "Base class - value: " << value << ", name: " << name 
                  << ", count: " << count << std::endl;
    }
};

// 派生类
class Derived : public Base {
private:
    int value;  // 隐藏基类的value成员
    double score;  // 新增成员
    static constexpr int count = 1;

public:
    Derived(int v1, int v2, const std::string& n, double s) 
        : Base(v1, n), value(v2), score(s) {}
    
    void display() {
        std::cout << "Derived class - Base::value: " << Base::value 
                  << ", Derived::value: " << value 
                  << ", name: " << name 
                  << ", score: " << score
                  << ", count: " << count 
                  << ", Base::count: " << Base::count << std::endl;
    }
};

int main() {
    // 创建基类对象
    Base base(100, "Base Object");
    std::cout << "Base object:" << std::endl;
    base.display();
    
    // 创建派生类对象
    Derived derived(200, 300, "Derived Object", 95.5);
    std::cout << "\nDerived object:" << std::endl;
    derived.display();
    
    return 0;
}
