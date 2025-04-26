#include <atomic>
#include <iostream>
#include <mutex>

using namespace std;

class Singleton {
public:
    static Singleton* getInstance() {
        // auto tmp = ptr_;
        if (!ptr_) {
            lock_guard<mutex> lck(mut_);
            if (!ptr_) {
                auto tmp = new Singleton();
                std::atomic_thread_fence(std::memory_order_acquire);
                ptr_ = tmp;
            }
        }
        return ptr_;
    }
    void Print() {
        cout << "singleton print" << endl;
    }


private:
    Singleton(){};
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    inline static Singleton* ptr_ = nullptr;
    inline static mutex mut_;
};

int main() 
{
    Singleton* ins = Singleton::getInstance();
    ins->Print();

    return 0;
}
