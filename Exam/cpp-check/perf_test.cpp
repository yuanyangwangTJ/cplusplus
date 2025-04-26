#include <iostream>
#include <chrono>
#include <thread>

void cpu_intensive_task() {
    volatile long sum = 0;
    for (long i = 0; i < 1e8; ++i) sum += i;  // 计算密集型任务
}

void io_task() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟 I/O
}

int main() {
    std::cout << "Starting perf test..." << std::endl;
    std::thread t1(cpu_intensive_task);
    std::thread t2(io_task);

    t1.join();
    t2.join();
    
    std::cout << "Perf test finished!" << std::endl;
    return 0;
}

/*
g++ -o perf_test perf_test.cpp -pthread -O2  # 线程支持 + 优化
perf record ./perf_test                      # 运行程序并采样
perf report                                  # 生成分析报告
*/