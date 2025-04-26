#include <cassert>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <tuple>
#include <vector>
#include <algorithm>

std::vector<uint32_t> solve(uint32_t n, std::vector<uint32_t> square)
{
    assert(n * n == square.size());
    std::vector<uint32_t> ans;
    std::vector<std::vector<uint32_t>> matrix(n, std::vector<uint32_t>(n));
    for (uint32_t i = 0; i < n; ++i) {
        for (uint32_t j = 0; j < n; ++j) {
            matrix[i][j] = square[i * n + j];
        }
    }

    auto rotate = [&](uint32_t x, uint32_t y) {
        std::vector<uint32_t> temp = {
            matrix[x][y], matrix[x][y + 1],
            matrix[x + 1][y + 1], matrix[x + 1][y]
        };
        matrix[x][y + 1] = temp[0];
        matrix[x + 1][y + 1] = temp[1];
        matrix[x + 1][y] = temp[2];
        matrix[x][y] = temp[3];
        ans.push_back(x * (n - 1) + y + 1);
    };

    auto find_position = [&](uint32_t val) {
        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                if (matrix[i][j] == val) {
                    return std::make_tuple(i, j);
                }
            }
        }
        return std::make_tuple(n, n);
    };

    for (uint32_t target = 1; target <= n * n; ++target) {
        auto [tx, ty] = find_position(target);
        auto ex = (target - 1) / n;
        auto ey = (target - 1) % n;

        while (tx != ex || ty != ey) {
            if (tx < ex) {
                rotate(tx, ty);
            } else if (tx > ex) {
                rotate(tx - 1, ty);
            } else if (ty < ey) {
                rotate(tx, ty);
            } else if (ty > ey) {
                rotate(tx, ty - 1);
            }
            std::tie(tx, ty) = find_position(target);
        }
    }

    return ans;
}

// A helper function to see a vector
void print_vector(const std::vector<uint32_t> &ans) noexcept
{
    std::printf("(size=%lu) [", ans.size());
    for (auto p : ans)
    {
        std::printf("%u,", p);
    }
    std::puts("]");
}

// A helper function to check your answer, and compare with the correct answer
bool check_answer(const uint32_t n, const std::vector<uint32_t> &input, const std::vector<uint32_t> &ans,
    const std::optional<std::vector<uint32_t>> &std_steps = std::nullopt) noexcept
{
    auto a = std::vector(n, std::vector<uint32_t>(n, 0));
    for (auto i = 0; i < n; i++)
    {
        for (auto j = 0; j < n; j++)
        {
            a[i][j] = input[i * n + j];
        }
    }
    for (auto op : ans)
    {
        auto x = (op - 1) / (n - 1);
        auto y = (op - 1) % (n - 1);
        std::swap(a[x][y], a[x][y + 1]);
        std::swap(a[x][y + 1], a[x + 1][y + 1]);
        std::swap(a[x + 1][y], a[x + 1][y + 1]);
    }

    for (auto i = 0; i < n; i++)
    {
        for (auto j = 0; j < n; j++)
        {
            if (a[i][j] != i * n + j + 1)
            {
                std::printf("!!!! answer is invalid\n");
                std::printf("!!!! expected %d at (%d, %d), but got %d\n", i * n + j + 1, i, j, a[i][j]);
                return false;
            }
        }
    }

    if (std_steps && ans.size() > std_steps->size())
    {
        std::printf("!!!! answer is accepted, but worse than std solution\n");
        return false;
    }

    return true;
}

int main()
{
    using example_type =
        std::tuple<uint32_t /*n*/, std::vector<uint32_t> /*square*/, std::vector<uint32_t> /*std_steps*/>;
    std::vector<example_type> examples{
        // 3x3
        {3, {4, 1, 3, 5, 2, 6, 7, 8, 9}, {1}},
        {3, {4, 1, 3, 8, 2, 5, 7, 9, 6}, {1, 4}},
        {3, {1, 3, 2, 4, 5, 6, 7, 8, 9}, {1, 3, 2, 4, 5, 6, 7, 8, 9}},

        // 5x5
        {5,
         {2, 7, 3, 4, 5, 1, 8, 13, 9, 10, 11, 6, 14, 19, 15, 16, 17, 12, 25, 24, 21, 22, 23, 20, 18},
         {16, 16, 11, 11, 11, 6, 6, 6, 1, 1, 1}},

        // 10x10
        {10,
         {11, 1,  3,  4,  5,  17, 16, 8,  9,  10, 12, 2,  13, 14, 15, 7,  6,  18, 19, 20, 21, 22, 23, 24, 36,
          35, 27, 28, 29, 30, 31, 32, 33, 34, 26, 25, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 59, 58, 50,
          51, 52, 53, 54, 55, 56, 57, 49, 48, 60, 61, 62, 63, 64, 65, 66, 67, 78, 68, 70, 71, 72, 73, 74, 75,
          76, 77, 79, 69, 80, 81, 82, 83, 84, 86, 96, 87, 98, 88, 90, 91, 92, 93, 94, 85, 95, 97, 99, 89, 100},
         {6, 44, 6, 62, 1, 44, 77, 77, 77, 80, 23, 23}},
    };

    for (auto &[n, square, std_steps] : examples)
    {
        auto ans = solve(n, square);
        check_answer(n, square, ans, std_steps);
    }

    return 0;
}