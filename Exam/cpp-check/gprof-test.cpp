#include <iostream>
#include <chrono>
#include <thread>

void slow_function() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时操作
}

void fast_function() {
    for (int i = 0; i < 1000000; ++i);  // 空循环，消耗 CPU
}

int main() {
    for (int i = 0; i < 5; ++i) {
        slow_function();
        fast_function();
    }
    std::cout << "Profiling Done!" << std::endl;
    return 0;
}
/*
g++ -pg -o gprof_test gprof_test.cpp    # 添加 -pg 选项编译
./gprof_test                             # 运行程序，生成 gmon.out
gprof gprof_test gmon.out > gprof_report.txt  # 生成分析报告
cat gprof_report.txt                     # 查看报告
*/