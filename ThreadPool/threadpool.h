#pragma once

#include <vector>
#include <queue>
#include <future>
#include <type_traits>
#include <functional>

class ThreadPool {
public:
	explicit ThreadPool(size_t=std::thread::hardware_concurrency());
	void Stop();

	template<typename Func, typename... Args>
	auto QueueJob(Func&&, Args&&...);
	//-> std::future<typename std::invoke_result_t<Func, Args...>>;

	void WaitUntilEmpty();

	~ThreadPool();

private:
	void ThreadLoop();

private:
	std::vector<std::thread> workers_;			// 线程池
	std::queue<std::function<void()>> tasks_;	// 任务队列

	// 同步变量
	std::mutex queue_mutex_;
	std::condition_variable mutex_condition_;
	std::atomic<bool> is_stop_{ false };
};

inline ThreadPool::ThreadPool(size_t threads_num) {
	workers_.resize(threads_num);
	for (auto& w : workers_) {
		w = std::thread(&ThreadPool::ThreadLoop, this);
	}
}

inline ThreadPool::~ThreadPool() {
	Stop();
}

// 无限循环，等待任务队列开启
inline void ThreadPool::ThreadLoop()
{
	while (true) {
		std::function<void()> task;
		{
			std::unique_lock lock(queue_mutex_);
			mutex_condition_.wait(lock, [this] {
				return is_stop_ || !tasks_.empty();
				});

			if (is_stop_ && tasks_.empty())
				return;

			task = std::move(tasks_.front());
			tasks_.pop();
		}
		task();
	}

}

template<typename Func, typename... Args>
inline auto ThreadPool::QueueJob(Func&& f, Args&&... args)
//-> std::future<typename std::invoke_result_t<Func, Args...>>
{
	using return_type = typename std::invoke_result_t<Func, Args...>;

	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
	);

	auto res = task->get_future();
	{
		std::scoped_lock lock(queue_mutex_);
		if (is_stop_)
			throw std::runtime_error("QueueJob on stopped ThreadPool");
		tasks_.emplace([task]() {(*task)(); });
	}
	mutex_condition_.notify_one();
	return res;
}

inline void ThreadPool::Stop() {
	is_stop_ = true;
	mutex_condition_.notify_all();

	// 结束所有线程
	for (auto& w : workers_) {
		if (w.joinable())
			w.join();
	}

	workers_.clear();
}

// 等待至所有task开始运行
// TODO:条件变量存在问题
inline void ThreadPool::WaitUntilEmpty()
{
	std::unique_lock lock(queue_mutex_);
	mutex_condition_.wait(lock, [this] {
		return tasks_.empty();
		});
}
