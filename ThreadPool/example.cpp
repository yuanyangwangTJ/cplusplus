#include <iostream>
#include <vector>
#include <chrono>

#include "threadpool.h"

int main()
{
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
    auto sum_fun = [](int a, int b) { return a + b; };
    ThreadPool pool(4);
    std::vector<std::future<int>> res;

    for (int i = 0; i < 3; ++i) {
        for (int j = 3; j < 8; ++j) {
            res.emplace_back(pool.QueueJob(sum_fun, i, j));
        }
    }

    // pool.WaitUntilEmpty();

    for (auto& r : res) {
        std::cout << r.get() << std::endl;
    }

    return 0;
}