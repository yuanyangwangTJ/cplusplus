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