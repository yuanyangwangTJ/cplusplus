#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

// Michael-Scott 队列
template<typename T>
class MPMCQueue {
private:
    struct Node {
        T data;
        std::atomic<Node*> next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    MPMCQueue() {
        Node* dummy = new Node(T());  // 哑节点（Dummy Node）
        head.store(dummy);
        tail.store(dummy);
    }

    ~MPMCQueue() {
        while (Node* node = head.load()) {
            head.store(node->next);
            delete node;
        }
    }

    void enqueue(const T& value) {
        Node* new_node = new Node(value);
        Node* old_tail;
        while (true) {
            old_tail = tail.load(std::memory_order_acquire);
            Node* next = old_tail->next.load(std::memory_order_acquire);
            if (old_tail == tail.load()) {
                if (next == nullptr) {
                    if (old_tail->next.compare_exchange_weak(next, new_node)) {
                        tail.compare_exchange_strong(old_tail, new_node);
                        return;
                    }
                } else {
                    tail.compare_exchange_strong(old_tail, next);
                }
            }
        }
    }

    bool dequeue(T& result) {
        while (true) {
            Node* old_head = head.load(std::memory_order_acquire);
            Node* old_tail = tail.load(std::memory_order_acquire);
            Node* next = old_head->next.load(std::memory_order_acquire);

            if (old_head == head.load()) {
                if (old_head == old_tail) {
                    if (next == nullptr) return false;  // 队列为空
                    tail.compare_exchange_strong(old_tail, next);
                } else {
                    if (head.compare_exchange_weak(old_head, next)) {
                        result = next->data;
                        delete old_head;
                        return true;
                    }
                }
            }
        }
    }
};


// 测试函数
void test_mpmc_queue() {
    MPMCQueue<int> queue;
    int item;

    // **测试 1: 空队列时 dequeue() 应返回 false**
    assert(queue.dequeue(item) == false);
    std::cout << "[TEST 1] dequeue() on empty queue: PASS\n";

    // **测试 2: 逐个 enqueue 和 dequeue，检查数据正确性**
    queue.enqueue(10);
    queue.enqueue(20);
    assert(queue.dequeue(item) && item == 10);
    assert(queue.dequeue(item) && item == 20);
    assert(queue.dequeue(item) == false);  // 再次 dequeue 应该失败
    std::cout << "[TEST 2] Single-threaded enqueue/dequeue correctness: PASS\n";

    // **测试 3: 多生产者-多消费者（MPMC）并发测试**
    constexpr int NUM_PRODUCERS = 4;
    constexpr int NUM_CONSUMERS = 4;
    constexpr int NUM_ITEMS = 100000;

    MPMCQueue<int> queue_mt;
    std::atomic<int> produced_count{0};
    std::atomic<int> consumed_count{0};

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // 生产者线程
    for (int p = 0; p < NUM_PRODUCERS; ++p) {
        producers.emplace_back([&]() {
            for (int i = 0; i < NUM_ITEMS; ++i) {
                queue_mt.enqueue(i);
                produced_count.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // 消费者线程
    for (int c = 0; c < NUM_CONSUMERS; ++c) {
        consumers.emplace_back([&]() {
            int value;
            for (int i = 0; i < NUM_ITEMS * NUM_PRODUCERS / NUM_CONSUMERS; ++i) {
                while (!queue_mt.dequeue(value)) { /* 等待数据 */ }
                consumed_count.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& p : producers) p.join();
    for (auto& c : consumers) c.join();

    // 生产和消费的总数应该相等
    assert(produced_count.load() == consumed_count.load());
    std::cout << "[TEST 3] Multi-threaded MPMC correctness: PASS\n";

    std::cout << "All tests passed!\n";
}

int main() {
    test_mpmc_queue();
    return 0;
}