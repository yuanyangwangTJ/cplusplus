#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

int sdk_loop(void* base, int id, 
    void (*callback)(char*, int, void*), void* arg) {
    std::cout << "sdk_loop" << std::endl;
    callback(nullptr, 0, arg);
    return 1;
}

class Foo {
public:
    void fun(char* str, int len) {
        std::cout << "fun: " << str << ", " << len << std::endl;
    }

    static void funWrapper(char* str, int len, void* obj) {
        Foo* foo = static_cast<Foo*>(obj);
        std::cout << foo->a << std::endl;
        foo->fun(str, len);
    }

    void test() {
        // auto handler = std::bind(&Foo::fun, _1, _2, _3);
        // void *p = this;
        // sdk_loop(nullptr, 1, handler, nullptr); // error
        sdk_loop(nullptr, 1, &Foo::funWrapper, this);
    }

private:
    int a = 5;
};

int main()
{
    Foo foo;
    foo.test();
    return 0;
}