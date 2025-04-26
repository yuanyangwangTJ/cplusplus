#include <cassert>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

int64_t solve(std::vector<std::vector<int32_t>> grid)
{
    // Enter your answer here.
    return {};
}

// A helper function to compare your answer with the correct answer
bool check_answer(int64_t ans, int64_t std_ans) noexcept
{
    if (ans == std_ans)
        return true;
    std::printf("!!!! Wrong answer: got %ld, expected %ld\n", ans, std_ans);
    return false;
}

std::vector<std::vector<int32_t>> example_grid(size_t k, size_t n)
{
    std::vector<std::vector<int32_t>> grid;
    grid.resize(k);
    const size_t random_seed = k * n;
    std::mt19937_64 gen(random_seed);

    for (size_t i = 0; i < k; ++i)
    {
        grid[i].resize(n);
        for (size_t j = 0; j < n; ++j)
            grid[i][j] = int32_t(gen() % 200) - 100;
    }

    return grid;
}

int main()
{
    // k=2, n=1
    check_answer(solve({{-9}, {4}}), -5);

    // k=2, n=3
    check_answer(solve({{2, 3, 4}, {10, -1, -2}}), 19);

    // k=2, n=4
    check_answer(solve({{3, 9, 6, 4}, {2, 10, 4, 5}}), 33);

    // k=3, n=5
    check_answer(solve({{5, 5, 7, 6, 0}, {1, 10, 1, 8, 6}, {9, 10, -5, -1, 4}}), 56);

    // k=2, n=1e3
    auto grid = example_grid(2, 1e3);
    check_answer(solve(std::move(grid)), 31310);

    // k=2, n=1e7
    grid = example_grid(2, 1e7);
    check_answer(solve(std::move(grid)), 328515000LL);

    // k=3, n=1e3
    grid = example_grid(3, 1e3);
    check_answer(solve(std::move(grid)), 50829);

    // k=3, n=1e7
    grid = example_grid(3, 1e7);
    check_answer(solve(std::move(grid)), 495065085LL);

    return 0;
}
