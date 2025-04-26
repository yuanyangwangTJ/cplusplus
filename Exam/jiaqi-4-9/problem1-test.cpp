#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstdint>

// Quickselect helper
int32_t quickselect(std::vector<int32_t> &a, int k) {
  int left = 0, right = a.size() - 1;
  while (left <= right) {
    int pivot_index = left + rand() % (right - left + 1);
    int32_t pivot = a[pivot_index];
    std::swap(a[pivot_index], a[right]);

    int store_index = left;
    for (int i = left; i < right; ++i) {
      if (a[i] < pivot)
        std::swap(a[i], a[store_index++]);
    }
    std::swap(a[store_index], a[right]);

    if (store_index == k)
      return a[store_index];
    else if (store_index < k)
      left = store_index + 1;
    else
      right = store_index - 1;
  }
  return -1; // Should never reach here
}

// Median
bool is_median(const std::vector<int32_t> &a, int32_t val) {
  std::vector<int32_t> copy = a;
  size_t n = copy.size();
  if (n & 1) {
    int32_t median = quickselect(copy, n / 2);
    return median == val;
  } else {
    int32_t m1 = quickselect(copy, n / 2 - 1);
    int32_t m2 = quickselect(copy, n / 2);
    return static_cast<int64_t>(m1) + m2 == static_cast<int64_t>(val) * 2;
  }
}

// Mean
bool is_mean(const std::vector<int32_t> &a, int32_t val) {
  int64_t sum = 0;
  for (auto num : a) {
    sum += num;
  }
  return sum == static_cast<int64_t>(val) * a.size();
}

// Mode
bool is_mode(const std::vector<int32_t> &a, int32_t val) {
  std::unordered_map<int32_t, int> freq;
  int max_freq = 0;

  for (auto num : a) {
    max_freq = std::max(max_freq, ++freq[num]);
  }

  return freq[val] == max_freq;
}

// Test cases
int main() {
  srand(static_cast<unsigned>(time(0)));

  // Test case 1
  std::vector<int32_t> a1 = {1, 2, 2, 3, 4};
  assert(!is_mean(a1, 2));
  assert(is_median(a1, 2));
  assert(is_mode(a1, 2));

  // Test case 2
  std::vector<int32_t> a2 = {1, 2, 3, 4};
  assert(!is_mean(a2, 2));
  assert(!is_median(a2, 2));
  assert(is_mode(a2, 2)); 

  // Test case 3
  std::vector<int32_t> a3 = {5, 5, 5, 5, 5};
  assert(is_mean(a3, 5));
  assert(is_median(a3, 5));
  assert(is_mode(a3, 5));

  // Test case 4
  std::vector<int32_t> a4 = {1, 2, 3, 3, 3, 4, 5};
  assert(is_mode(a4, 3));
  assert(!is_mode(a4, 2));
  assert(is_median(a4, 3));
  assert(is_mean(a4, 3)); // sum = 21, size = 7, mean = 3

  // Test case 5
  std::vector<int32_t> a5 = {1, 1, 2, 2, 3, 3};
  assert(is_mode(a5, 1)); // multiple modes: 1, 2, 3
  assert(is_mean(a5, 2));  // sum = 12, size = 6
  assert(is_median(a5, 2)); // sorted: 1,1,2,2,3,3 → median = (2+2)/2 = 2

  std::cout << "All tests passed!\n";
  return 0;
}
