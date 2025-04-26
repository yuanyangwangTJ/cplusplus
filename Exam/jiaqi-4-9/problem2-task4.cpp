#include <iostream>
#include <vector>
#include <random>
#include <chrono>

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

int main() {
    const std::size_t num_keys = 100000;
    const std::size_t batch_size = 1000;
    const int value_min = 1;
    const int value_max = 100;
    const int value_seed = 42;

    std::vector<int> keys(num_keys, 1);  // Just an example
    std::vector<int> out_keys(num_keys * batch_size);
    std::vector<int> out_values(num_keys * batch_size);

    auto start = std::chrono::high_resolution_clock::now();
    
    generate(keys, batch_size, value_min, value_max, value_seed, out_keys.data(), out_values.data());
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    // Calculate throughput (operations per second)
    double operations = num_keys * batch_size * 2; // Two operations per iteration (key and value)
    double throughput = operations / duration.count(); // operations per second

    std::cout << "Throughput: " << throughput << " operations per second" << std::endl;

    return 0;
}
