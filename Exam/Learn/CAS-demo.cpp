#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> shared_data(0); // 共享变量

void cas_thread(int expected, int new_value) {
    int temp = expected;
    if (shared_data.compare_exchange_weak(temp, new_value)) {
        std::cout << "Thread " << std::this_thread::get_id()
                  << " successfully updated shared_data to " << new_value << std::endl;
    } else {
        std::cout << "Thread " << std::this_thread::get_id()
                  << " failed, current value is " << shared_data.load() << std::endl;
    }
}

int main() {
    std::thread t1(cas_thread, 0, 100);
    std::thread t2(cas_thread, 0, 200);
    
    t1.join();
    t2.join();
    
    std::cout << "Final value: " << shared_data.load() << std::endl;
    return 0;
}
