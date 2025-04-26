#include <cassert>
#include <cstddef>
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T, size_t Capacity>
class SPSCQueue {
public:
    SPSCQueue() : buf_(Capacity) {}

    bool enqueue(const T& item) {
        auto cur_size = head_.load(std::memory_order_relaxed);
        auto next_size = (cur_size + 1) % Capacity;
        if (next_size == tail_.load(std::memory_order_acquire)) {
            return false;
        }
        buf_[cur_size] = item;
        head_.store(next_size, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item) {
        auto cur_tail = tail_.load(std::memory_order_relaxed);
        if (cur_tail == head_.load(std::memory_order_acquire)) {
            return false;   // empty
        }
        item = buf_[cur_tail];
        tail_.store((cur_tail + 1) % Capacity, std::memory_order_release);
        return true;
    }

private:
    std::vector<T> buf_;
    std::atomic<size_t> head_{0};   // proudcer
    std::atomic<size_t> tail_{0};   // consumer

};


// 测试函数
void test_spsc_queue() {
    constexpr size_t QUEUE_CAPACITY = 8;
    SPSCQueue<int, QUEUE_CAPACITY> queue;

    int item;

    // **测试 1: 空队列时 dequeue() 应返回 false**
    assert(queue.dequeue(item) == false);
    std::cout << "[TEST 1] dequeue() on empty queue: PASS\n";

    // **测试 2: 逐个 enqueue 和 dequeue，检查数据正确性**
    assert(queue.enqueue(10) == true);
    assert(queue.enqueue(20) == true);
    assert(queue.dequeue(item) == true && item == 10);
    assert(queue.dequeue(item) == true && item == 20);
    assert(queue.dequeue(item) == false);  // 再次 dequeue 应该失败
    std::cout << "[TEST 2] Single-threaded enqueue/dequeue correctness: PASS\n";

    // **测试 3: 队列满时 enqueue() 应该失败**
    for (int i = 0; i < QUEUE_CAPACITY - 1; ++i) {
        assert(queue.enqueue(i) == true);
    }
    assert(queue.enqueue(999) == false);  // 应该失败，因为队列已满
    std::cout << "[TEST 3] enqueue() on full queue: PASS\n";

    // **测试 4: 多线程 SPSC 并发测试**
    SPSCQueue<int, 1024> queue_mt;
    constexpr int NUM_ITEMS = 1000000;
    std::thread producer([&]() {
        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!queue_mt.enqueue(i)) { /* 等待空间 */ }
        }
    });

    std::thread consumer([&]() {
        int last_value = -1;
        for (int i = 0; i < NUM_ITEMS; ++i) {
            int value;
            while (!queue_mt.dequeue(value)) { /* 等待数据 */ }
            assert(value == last_value + 1);
            last_value = value;
        }
    });

    producer.join();
    consumer.join();
    std::cout << "[TEST 4] Multi-threaded SPSC correctness: PASS\n";

    std::cout << "All tests passed!\n";
}

int main() {
    test_spsc_queue();
    return 0;
}