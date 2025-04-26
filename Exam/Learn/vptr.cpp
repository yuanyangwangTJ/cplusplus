#include <iostream>
using namespace std;

class A {
public:
    A() { cout << "A constructor" << endl; }
    void show() { cout << "A's show()" << endl; }
};

class B : virtual public A {
public:
    B() { cout << "B constructor" << endl; }
};

class C : virtual public A {
public:
    C() { cout << "C constructor" << endl; }
};

class D : public B, public C {
public:
    D() { cout << "D constructor" << endl; }
};

class E : public A {
    public:
    E() { cout << "E constructor" << endl; }
};

int main() {
    D d;
    d.show();  // 调用 A 类的方法，虽然 A 被继承两次，但由于虚继承，只有一份 A 的实例

    E e;
    e.show();
    return 0;
}
