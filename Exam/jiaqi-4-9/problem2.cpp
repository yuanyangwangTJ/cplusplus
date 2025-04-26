#include <vector>
#include <random>
#include <cstddef>
#include <cassert>
#include <iostream>

// PRNG generate
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

void test_generate_basic() {
    std::vector<int> keys = {1, 2, 3};
    std::size_t batch_size = 2;
    int value_min = 10, value_max = 20;
    int seed = 42;

    std::size_t total_size = keys.size() * batch_size;
    std::vector<int> keys_out(total_size);
    std::vector<int> values_out(total_size);

    generate(keys, batch_size, value_min, value_max, seed, keys_out.data(), values_out.data());

    // check key
    for (std::size_t i = 0; i < keys.size(); ++i) {
        for (std::size_t j = 0; j < batch_size; ++j) {
            assert(keys_out[i * batch_size + j] == keys[i]);
        }
    }

    // check value range
    for (int val : values_out) {
        assert(val >= value_min && val <= value_max);
    }

    std::cout << "test_generate_basic passed.\n";
}

void test_same_seed_same_output() {
    std::vector<int> keys = {9};
    std::size_t batch_size = 5;
    int value_min = 0, value_max = 100;
    int seed = 12345;

    std::vector<int> k1(5), v1(5);
    std::vector<int> k2(5), v2(5);

    generate(keys, batch_size, value_min, value_max, seed, k1.data(), v1.data());
    generate(keys, batch_size, value_min, value_max, seed, k2.data(), v2.data());

    for (int i = 0; i < 5; ++i) {
        assert(k1[i] == k2[i]);
        assert(v1[i] == v2[i]);
    }

    std::cout << "test_same_seed_same_output passed.\n";
}

void test_diff_seed_diff_output() {
    std::vector<int> keys = {5};
    std::size_t batch_size = 5;
    int value_min = 0, value_max = 100;

    std::vector<int> v1(5), v2(5);
    std::vector<int> dummy(5);  // key is not important

    generate(keys, batch_size, value_min, value_max, 100, dummy.data(), v1.data());
    generate(keys, batch_size, value_min, value_max, 200, dummy.data(), v2.data());

    bool different = false;
    for (int i = 0; i < 5; ++i) {
        if (v1[i] != v2[i]) {
            different = true;
            break;
        }
    }
    assert(different);  // maybe same

    std::cout << "test_diff_seed_diff_output passed.\n";
}

void test_value_min_equals_max() {
    std::vector<int> keys = {1, 2};
    std::size_t batch_size = 3;
    int value_min = 42, value_max = 42;
    int seed = 0;

    std::size_t size = keys.size() * batch_size;
    std::vector<int> keys_out(size);
    std::vector<int> values_out(size);

    generate(keys, batch_size, value_min, value_max, seed, keys_out.data(), values_out.data());

    for (int val : values_out) {
        assert(val == 42);
    }

    std::cout << "test_value_min_equals_max passed.\n";
}

int main() {
    test_generate_basic();
    test_same_seed_same_output();
    test_diff_seed_diff_output();
    test_value_min_equals_max();
    std::cout << "All tests passed!\n";
    return 0;
}
