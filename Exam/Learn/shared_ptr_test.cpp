#include <iostream>
#include <memory>

struct A {
    virtual ~A() = default;
    void showA() { std::cout << "A::showA()" << std::endl; }
};

struct B : public A {
    void showB() { std::cout << "B::showB()" << std::endl; }
};

int main() {
    std::shared_ptr<A> pa = std::make_shared<B>();  // 共享指针管理 B 对象
    std::shared_ptr<B> pb(pa, static_cast<B*>(pa.get())); // 共享 control block

    std::cout << "pa use count: " << pa.use_count() << std::endl; // 2
    std::cout << "pb use count: " << pb.use_count() << std::endl; // 2

    pb->showB(); // ✅ 安全调用 B 的方法

    return 0;
}
