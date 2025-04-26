#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(size_t size = std::thread::hardware_concurrency());
    template<typename Fun, typename... Args>
    auto QueueJob(Fun&&, Args&&...)
    -> std::future<std::invoke_result_t<Fun, Args...>>;

    void Stop();
    void WaitUntilEmpty();
    
    // ~ThreadPool() {
    //     Stop();
    // }

private:
    void ThreadLoop();

private:
    std::queue<std::function<void()>> tasks_;
    std::vector<std::thread> pool_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic_bool is_stop_;
};

ThreadPool::ThreadPool(size_t size) : is_stop_(false) {
    pool_.resize(size);
    for (auto& w : pool_) {
        w = std::thread(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lck(mutex_);
            cv_.wait(lck, [this] {
                return is_stop_ || !tasks_.empty();
            });

            if (is_stop_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}

template<typename Fun, typename... Args>
auto ThreadPool::QueueJob(Fun&& fun, Args&&... args)
-> std::future<std::invoke_result_t<Fun, Args...>> {
    using return_type = std::invoke_result_t<Fun, Args...>;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...)
    );

    auto res = task->get_future();
    {
        std::lock_guard lck(mutex_);
        if (is_stop_) {
            throw std::runtime_error("pool stopped!");
        }
        tasks_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return res;
}

void ThreadPool::Stop() {
	is_stop_ = true;
	cv_.notify_all();

	// 结束所有线程
	for (auto& w : pool_) {
		if (w.joinable())
			w.join();
	}

	pool_.clear();
}

void ThreadPool::WaitUntilEmpty() {
    std::unique_lock lck(mutex_);
    cv_.wait(lck, [this] {
        return tasks_.empty();
    });
}

int main() {
    auto sum_fun = [](int a, int b) { return a + b; };
    ThreadPool pool;
    std::vector<std::future<int>> res;

    for (int i = 0; i < 3; ++i) {
        for (int j = 3; j < 8; ++j) {
            res.emplace_back(pool.QueueJob(sum_fun, i, j));
        }
    }

    // pool.WaitUntilEmpty();

    for (auto& r : res) {
        cout << r.get() << endl;
    }

    pool.Stop();

    // return 0;
    // ThreadPool pool(4);
    // std::vector< std::future<int> > results;

    // for (int i = 0; i < 8; ++i) {
    //     results.emplace_back(
    //         pool.QueueJob([i] {
    //             std::cout << "hello " << i << std::endl;
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //             std::cout << "world " << i << std::endl;
    //             return i * i;
    //             })
    //     );
    // }

    // for (auto&& result : results)
    //     std::cout << result.get() << ' ';
    // std::cout << std::endl;

    return 0;
}
