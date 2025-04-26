#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>
#include <memory>

void generate(const std::vector<int>& keys,
              std::size_t batch_size,
              int value_min,
              int value_max,
              int value_seed,
              int* keys_out,
              int* values_out) noexcept {
    std::mt19937 rng(value_seed);  
    std::uniform_int_distribution<int> dist(value_min, value_max);

    std::size_t index = 0;
    for (int key : keys) {
        for (std::size_t i = 0; i < batch_size; ++i) {
            keys_out[index] = key;
            values_out[index] = dist(rng);
            ++index;
        }
    }
}

// ----------------------------
// All generator variants here
// ----------------------------

void generate_fast1(const std::vector<int> &keys, std::size_t batch_size,
                    int value_min, int value_max, int value_seed,
                    int *keys_out, int *values_out) noexcept {
    std::mt19937 rng(value_seed);
    std::size_t index = 0;
    for (int key : keys) {
        for (std::size_t i = 0; i < batch_size; ++i) {
            keys_out[index] = key;
            values_out[index++] = value_min + (rng() % (value_max - value_min + 1));
        }
    }
}

void generate_fast2(const std::vector<int>& keys,
                       std::size_t batch_size,
                       int value_min,
                       int value_max,
                       int value_seed,
                       int* keys_out,
                       int* values_out) noexcept {
    std::mt19937 rng(value_seed);
    std::uniform_int_distribution<int> dist(value_min, value_max);

    std::size_t index = 0;
    for (int key : keys) {
        std::size_t i = 0;
        for (; i + 4 <= batch_size; i += 4) {
            keys_out[index] = key;
            values_out[index++] = dist(rng);
            keys_out[index] = key;
            values_out[index++] = dist(rng);
            keys_out[index] = key;
            values_out[index++] = dist(rng);
            keys_out[index] = key;
            values_out[index++] = dist(rng);
        }
        for (; i < batch_size; ++i) {
            keys_out[index] = key;
            values_out[index++] = dist(rng);
        }
    }
}


void generate_fast3(const std::vector<int> &keys, std::size_t batch_size,
                    int value_min, int value_max, int value_seed,
                    int *keys_out, int *values_out) noexcept {
    std::minstd_rand rng(value_seed);
    std::size_t index = 0;
    for (int key : keys) {
        for (std::size_t i = 0; i < batch_size; ++i) {
            keys_out[index] = key;
            values_out[index++] = value_min + (rng() % (value_max - value_min + 1));
        }
    }
}

// Multi-threaded generate function with jthread
void generate_parallel_jthread(const std::vector<int>& keys,
                                std::size_t batch_size,
                                int value_min,
                                int value_max,
                                int value_seed,
                                int* keys_out,
                                int* values_out,
                                std::size_t start_index,
                                std::size_t end_index) noexcept {
    std::mt19937 rng(value_seed);
    std::uniform_int_distribution<int> dist(value_min, value_max);

    std::size_t index = start_index;
    for (std::size_t key_idx = start_index; key_idx < end_index; ++key_idx) {
        int key = keys[key_idx];
        for (std::size_t i = 0; i < batch_size; ++i) {
            keys_out[index] = key;
            values_out[index++] = dist(rng);
        }
    }
}

void generate_fast4(const std::vector<int>& keys,
              std::size_t batch_size,
              int value_min,
              int value_max,
              int value_seed,
              int* keys_out,
              int* values_out) noexcept {
    const std::size_t num_keys = keys.size();
    const std::size_t num_threads = std::jthread::hardware_concurrency();  // Use hardware concurrency for optimal thread count

    // Calculate the range of indices for each thread
    std::vector<std::jthread> threads;
    std::size_t range_size = num_keys / num_threads;

    for (std::size_t i = 0; i < num_threads; ++i) {
        std::size_t start_index = i * range_size;
        std::size_t end_index = (i == num_threads - 1) ? num_keys : (i + 1) * range_size;

        // Create jthread and pass the function
        threads.emplace_back(generate_parallel_jthread, std::ref(keys), batch_size, value_min, value_max, value_seed,
                             keys_out, values_out, start_index, end_index);
    }
    // jthreads are automatically joined when they go out of scope
}


// ----------------------------
// Benchmark helper
// ----------------------------

template <typename GenFunc>
void benchmark(const std::string &name, GenFunc func,
               const std::vector<int> &keys, std::size_t batch_size,
               int value_min, int value_max, int value_seed) {
    std::size_t output_size = keys.size() * batch_size;
    // std::unique_ptr<int[]> keys_out(new int[output_size]);
    // std::unique_ptr<int[]> values_out(new int[output_size]);

    // auto start = std::chrono::high_resolution_clock::now();
    // func(keys, batch_size, value_min, value_max, value_seed, keys_out.get(), values_out.get());
    // auto end = std::chrono::high_resolution_clock::now();

    std::vector<int> keys_out(output_size);
    std::vector<int> values_out(output_size);

    auto start = std::chrono::high_resolution_clock::now();
    func(keys, batch_size, value_min, value_max, value_seed, keys_out.data(), values_out.data());
    auto end = std::chrono::high_resolution_clock::now();

    double duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << name << " took " << duration_ms << " ms\n";
}

// ----------------------------
// Main: Run all benchmarks
// ----------------------------

int main() {
    // Test parameters
    const std::size_t num_keys = 1'000'000;
    const std::size_t batch_size = 32;
    const int value_min = 1;
    const int value_max = 100;
    const int value_seed = 42;

    // Generate keys
    std::vector<int> keys(num_keys);
    std::iota(keys.begin(), keys.end(), 0); // 0, 1, 2, ...

    // Run benchmarks
    benchmark("generate (origin)", generate, keys, batch_size, value_min, value_max, value_seed);
    benchmark("generate_fast1 (raw rng)", generate_fast1, keys, batch_size, value_min, value_max, value_seed);
    benchmark("generate_fast2 (loop unrolled)", generate_fast2, keys, batch_size, value_min, value_max, value_seed);
    benchmark("generate_fast3 (minstd_rand)", generate_fast3, keys, batch_size, value_min, value_max, value_seed);
    benchmark("generate_fast4 (multi-thread)", generate_fast4, keys, batch_size, value_min, value_max, value_seed);

    return 0;
}
