#include <iostream>

using namespace std;

class Singleton {
public:
    static Singleton& getInstance()
    {
        static Singleton ins;
        return ins;
    }

    void Print() {
        cout << "single print" << endl;
    }

    void Print2() {
        auto& ins = Singleton::getInstance();
        ins.Print();
    }

private:
    Singleton() {};    // 构造函数必须私有，不是delete
    ~Singleton() {};

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&&) = delete;

};

int main()
{
    auto& ins = Singleton::getInstance();
    ins.Print();
    ins.Print2();

    return 0;
}