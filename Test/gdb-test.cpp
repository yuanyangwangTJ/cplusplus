#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex print_mutex;
std::atomic<bool> should_terminate(false);
std::vector<std::thread> g_threads; // 全局存储线程，防止提前销毁

void worker_function(int id) {
    int count = 0;
    while (!should_terminate && count < 10) { // 执行10次循环后退出
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << "线程 " << id << " 正在执行，计数: " << count << std::endl;
        }
        
        // 模拟工作负载
        std::this_thread::sleep_for(std::chrono::seconds(20));
        count++;
    }
    
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "线程 " << id << " 执行完毕" << std::endl;
    }
}

// 这是我们将在其前后设置断点的函数
void create_threads(int num_threads) {
    std::cout << "准备创建 " << num_threads << " 个线程..." << std::endl;
    
    // 创建多个线程
    for (int i = 0; i < num_threads; ++i) {
        g_threads.push_back(std::thread(worker_function, i));
    }
    
    std::cout << "已创建 " << num_threads << " 个线程，它们正在后台运行" << std::endl;
    // 注意：这里不再join线程，让它们在后台继续运行
}

int main() {
    std::cout << "程序启动，主线程ID: " << std::this_thread::get_id() << std::endl;
    
    // 调用创建线程的函数
    create_threads(3);
    
    std::cout << "主线程继续执行其他工作..." << std::endl;
    
    // 主线程也做一些工作
    for (int i = 0; i < 5; i++) {
        std::cout << "主线程执行中: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "主线程工作完成，等待后台线程..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // 设置终止标志
    should_terminate = true;
    
    // 等待所有线程完成
    for (auto& t : g_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    std::cout << "程序即将结束" << std::endl;
    return 0;
}
