// private static test
#include <iostream>

class Foo {
private:
    constexpr static int a = 1;
};

class Bar : public Foo {
public:
    void fun() {
    }
};

int main() 
{
    Bar b;
    b.fun();
}