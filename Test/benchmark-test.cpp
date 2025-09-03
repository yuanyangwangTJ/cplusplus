#include <benchmark/benchmark.h>
#include <vector>

// 一个简单的函数用于基准测试
static void BM_VectorPushBack(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_VectorPushBack)->Range(8, 8<<10);

// 另一个基准测试函数，测试vector预分配空间的性能
static void BM_VectorPushBackReserve(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_VectorPushBackReserve)->Range(8, 8<<10);

// 字符串连接的基准测试
static void BM_StringConcatenation(benchmark::State& state) {
    std::string str = "hello";
    for (auto _ : state) {
        std::string result;
        for (int i = 0; i < state.range(0); ++i) {
            result += str;
        }
    }
}
BENCHMARK(BM_StringConcatenation)->Range(2, 16);

BENCHMARK_MAIN();
