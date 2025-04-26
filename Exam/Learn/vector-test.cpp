#include <iostream>
#include <vector>

using namespace std;

class A {
public:
    A(int a) : a_(a) {
        cout << "construct: " << a_ << endl;
    }

    A(const A& rhs) {
        a_ = rhs.a_;
        cout << "copy construct: " << a_ << endl;
    }

    A(A&& rhs) noexcept {   // 
        a_ = rhs.a_;
        cout << "move construct: " << a_ << endl;
    }

private:
    int a_;
    string s_;
};

int main()
{
    // A b(1);
    vector<A> vec;
    // vec.reserve(2);
    vec.push_back(1);
    cout << "--------------" << endl;
    // 因为涉及到扩容，注意移动构造函数必须 noexcept，否则无法扩容时无法调用移动构造，只会调用拷贝构造函数
    vec.emplace_back(2);
}