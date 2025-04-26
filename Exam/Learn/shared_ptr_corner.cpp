#include <functional>
#include <iostream>
#include <memory>
#include <ratio>

using namespace std;

class A {
    public:
    ~A() {
        cout << "A" << endl;
    }
};

class B : public A {
public:
    ~B() {
        cout << "B" << endl;
    }
};


template <typename T>
class MySharedV2
{
public:
    template <typename T2>
    MySharedV2(T2 *p)
    {
        data_ = p;
        deleter_ = [p](){ delete p;};
    }
    ~MySharedV2()
    {
        deleter_();
    }
    T* operator->()
    {
        return data_;
    }
private:
    std::function<void()> deleter_;
    T* data_ = nullptr;
};


int main()
{
    /*
    shared_ptr记录了析构函数，析构输出BA，但是这会导致shared_ptr结构体更大
    */
    shared_ptr<A> ptr(new B);
    unique_ptr<A> uptr(new B);
    MySharedV2<A> myptr(new B);
    return 0;
}