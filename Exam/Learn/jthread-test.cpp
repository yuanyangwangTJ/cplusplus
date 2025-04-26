#include <iostream>
#include <thread>
#include <utility>

void worker() {
    std::cout << "Worker thread running...\n";
}

int main() {
    std::jthread jt(worker);
    
    // std::thread t = std::move(jt);  // 转换为 std::thread
    // jt.detach();  // 手动 detach

    std::cout << "Main thread finished.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 确保子线程有时间执行

    return 0;
}
