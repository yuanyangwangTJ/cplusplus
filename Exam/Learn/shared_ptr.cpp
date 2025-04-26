#include <iostream>
#include <atomic>
#include <vector>
#include <thread>

using namespace std;

template<typename T>
class SharedPtr {
public:
    SharedPtr() : _ptr(nullptr), _ref_cnt(nullptr) {}
    
    explicit SharedPtr(T* p) : _ptr(p), _ref_cnt(p ? new atomic<size_t>(1) : nullptr) {

    }
    ~SharedPtr() {
        release();
    }

    // 拷贝构造函数
    SharedPtr(const SharedPtr<T>& other) {
        _ptr = other._ptr;
        _ref_cnt = other._ref_cnt;
        if (_ref_cnt) {
            _ref_cnt->fetch_add(1, std::memory_order_relaxed);
        }
    }

    // 拷贝赋值运算符
    SharedPtr<T>& operator=(const SharedPtr<T>& other) {
        if (this != &other) {
            release();
            _ptr = other._ptr;
            _ref_cnt = other._ref_cnt;
            if (_ref_cnt) {
                _ref_cnt->fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }

    // 移动构造函数
    SharedPtr(SharedPtr<T>&& other) noexcept {
        _ptr = other._ptr;
        _ref_cnt = other._ref_cnt;
        other._ptr = nullptr;
        other._ref_cnt = nullptr;
    }

    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept {
        if (this != &other) {
            release();
            _ptr = other._ptr;
            _ref_cnt = other._ref_cnt;
            other._ptr = nullptr;
            other._ref_cnt = nullptr;
        }
        return *this;
    }

    // 解引用
    T& operator*() const {
        return *_ptr;
    }

    T* operator->() const {
        return _ptr;
    }

    // 获取引用计数
    size_t use_count() {
        return _ref_cnt ? _ref_cnt->load(std::memory_order_acquire) : 0;
    }

    T* get() const {
        return _ptr;
    }

    // 重置
    void reset(T *p = nullptr) {
        release();
        _ptr = p;
        _ref_cnt = p ? new atomic<size_t>(1) : nullptr;
    }

private:
    T *_ptr;
    atomic<size_t>* _ref_cnt;

    void release() {
        if (_ref_cnt && _ref_cnt->fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete _ptr;
            delete _ref_cnt;
        }
    }
};

void testSharedPtrThreadSafety() {
    SharedPtr<int> ptr(new int(42));
    // 创建多个线程，每个线程都增加或减少引用计数
    constexpr int NUM = 10;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM; ++i) {
        threads.emplace_back([&ptr]() {
            for (int i = 0; i < 10000; ++i) {
                SharedPtr<int> local_ptr(ptr);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // 检查引用计数正确性
    cout << "use_count: " << ptr.use_count() << endl;
    if (ptr.use_count() == 1) {
        cout << "thread safe" << endl;
    }
    else {
        cout << "not thread safe" << endl;
    }
}


int main()
{
    testSharedPtrThreadSafety();

    return 0;
}