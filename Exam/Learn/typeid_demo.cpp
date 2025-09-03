#include <iostream>
#include <typeinfo>
#include <string>

class Base {
public:
    virtual ~Base() = default;  // 使Base成为多态类
    virtual void func() { std::cout << "Base::func()" << std::endl; }
};

class Derived : public Base {
public:
    void func() override { std::cout << "Derived::func()" << std::endl; }
};

class NonPolymorphic {
public:
    void func() { std::cout << "NonPolymorphic::func()" << std::endl; }
};

int main() {
    std::cout << "=== typeid 基本用法 ===" << std::endl;
    
    // 1. 基本类型
    int i = 42;
    double d = 3.14;
    std::string s = "hello";
    
    std::cout << "int类型: " << typeid(int).name() << std::endl;
    std::cout << "double类型: " << typeid(double).name() << std::endl;
    std::cout << "string类型: " << typeid(std::string).name() << std::endl;
    
    std::cout << "变量i的类型: " << typeid(i).name() << std::endl;
    std::cout << "变量d的类型: " << typeid(d).name() << std::endl;
    std::cout << "变量s的类型: " << typeid(s).name() << std::endl;
    
    std::cout << "\n=== 类型比较 ===" << std::endl;
    
    // 2. 类型比较
    if (typeid(i) == typeid(int)) {
        std::cout << "i 是 int 类型" << std::endl;
    }
    
    if (typeid(d) == typeid(double)) {
        std::cout << "d 是 double 类型" << std::endl;
    }
    
    std::cout << "\n=== hash_code 使用 ===" << std::endl;
    
    // 3. hash_code - 在variant中使用的方式
    std::cout << "int的hash_code: " << typeid(int).hash_code() << std::endl;
    std::cout << "double的hash_code: " << typeid(double).hash_code() << std::endl;
    std::cout << "string的hash_code: " << typeid(std::string).hash_code() << std::endl;
    
    std::cout << "\n=== 多态类型识别 ===" << std::endl;
    
    // 4. 多态类型（虚函数）
    Base* base_ptr = new Derived();
    Base& base_ref = *base_ptr;
    
    std::cout << "静态类型: " << typeid(Base).name() << std::endl;
    std::cout << "通过指针的动态类型: " << typeid(*base_ptr).name() << std::endl;
    std::cout << "通过引用的动态类型: " << typeid(base_ref).name() << std::endl;
    
    // 比较类型
    if (typeid(*base_ptr) == typeid(Derived)) {
        std::cout << "指针指向的对象是Derived类型" << std::endl;
    }
    
    std::cout << "\n=== 非多态类型 ===" << std::endl;
    
    // 5. 非多态类型
    NonPolymorphic* non_poly_ptr = new NonPolymorphic();
    std::cout << "非多态类型: " << typeid(*non_poly_ptr).name() << std::endl;
    
    std::cout << "\n=== 模板中的typeid ===" << std::endl;
    
    // 6. 模板中的使用
    auto print_type = [](auto&& val) {
        std::cout << "类型: " << typeid(val).name() 
                  << ", hash: " << typeid(val).hash_code() << std::endl;
    };
    
    print_type(42);
    print_type(3.14);
    print_type("hello");
    
    // 清理内存
    delete base_ptr;
    delete non_poly_ptr;
    
    return 0;
} 