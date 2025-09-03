#include <benchmark/benchmark.h>
#include <string>

// 模拟本地消息发送函数
void SendLocalMessage(const std::string& msg) {
    // 模拟消息发送
    volatile int sink = 0;
    for (char c : msg) {
        sink += c;
    }
}

// 基准测试：测试不同大小消息的发送性能
static void BM_LocalMessageTransfer(benchmark::State& state) {
    // 生成测试消息
    std::string message(state.range(0), 'a');
    
    for (auto _ : state) {
        SendLocalMessage(message);
    }
    
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}

// 注册基准测试
BENCHMARK(BM_LocalMessageTransfer)
    ->Range(8, 8<<10)  // 测试8B到8KB大小的消息
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();

