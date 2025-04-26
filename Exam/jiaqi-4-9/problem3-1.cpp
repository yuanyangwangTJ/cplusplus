#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <random>
#include <string_view>
#include <vector>

namespace
{

void invalidate_cache() noexcept
{
    constexpr size_t size = 1ULL * 1024 * 1024 * 1024; // 1GB
    auto buffer = std::make_unique<volatile char[]>(size);
    for (size_t i = 0; i < size; i += 64)
        buffer[i] = 0;
}

struct Query
{
    uint32_t k;
    float lookup;
};

// Test state.
struct State
{
public:
    std::vector<std::vector<float>> data;
    std::vector<Query> queries;
    std::vector<int64_t> answers;

private:
    std::vector<int64_t> std_answers_;
    bool need_validate_;
    const char *algo_name_;

    using hrc_clock = std::chrono::high_resolution_clock;
    hrc_clock::time_point start_time_;

    static constexpr uint32_t random_seed = 0;

public:
    State(uint32_t data_batch_size, uint32_t data_size, uint32_t query_batch_size, bool need_validate)
        : need_validate_(need_validate)
    {
        std::printf("generating test data... ");
        assert(query_batch_size >= data_batch_size);
        assert(data_size >= 8);

        // Create arrays in ascending order
        // NOTE Please assume they are generated randomly. Another random methods will be used for the final evaluation.
        data.resize(data_batch_size);
        data.front().resize(data_size);
        for (uint32_t i = 0; i < data_size; ++i)
            data.front()[i] = float(i);
        for (auto &d : data)
            d = data.front();

        // Your answers to the queries will be saved in this vector.
        answers.resize(query_batch_size);

        // Create queries
        std::mt19937_64 generator(random_seed); /* NOLINT(cert-*) */
        std::uniform_real_distribution<float> distribution(-1.0f, float(data_size));
        queries.resize(query_batch_size);
        if (need_validate)
            std_answers_.resize(query_batch_size);
        const uint32_t query_per_data_batch = query_batch_size / data_batch_size;
        for (uint32_t i = 0; i < query_batch_size; ++i)
        {
            const uint32_t k = std::min(i / query_per_data_batch, data_batch_size - 1);
            const float lookup = distribution(generator);
            queries[i] = {.k = k, .lookup = lookup};

            if (need_validate)
                std_answers_[i] = std::lower_bound(data[k].begin(), data[k].end(), lookup) - data[k].begin();
        }

        std::printf("done: n=%u, size=%u, m=%u\n", data_batch_size, data_size, query_batch_size);
    }

    void start(const char *algo_name) noexcept
    {
        this->algo_name_ = algo_name;
        invalidate_cache();
        start_time_ = hrc_clock::now();
    }

    void stop() noexcept
    {
        auto stop_time = hrc_clock::now();
        std::chrono::duration<double, std::nano> elapsed = stop_time - start_time_;

        auto divby = std::log2((double)data.front().size());
        auto perop = elapsed.count() / divby / double(queries.size());

        std::printf("%g ns per lookup/log2(size)\n", perop);

        if (need_validate_ && answers != std_answers_)
        {
            std::printf("!!!! Wrong answer\n");
        }
    }
};

int usage(const char *msg = nullptr)
{
    std::puts("");
    if (msg)
        std::puts(msg);

    std::printf("  Usage: bisect_test <algo> <size>\n\n"
                "   <algo>: naive t0 t1 t2 t3\n"
                "   <size>: 8 big\n\n");
    return 1;
}

struct test_param
{
    uint32_t data_batch_size;
    uint32_t data_size;
    uint32_t query_batch_size;
    bool need_validate;
};

using test_function_type = void (*)(State &s) noexcept;

void test_naive(State &s) noexcept
{
    for (uint32_t i = 0, qn = s.queries.size(); i != qn; ++i)
    {
        const auto [k, lookup] = s.queries[i];
        s.answers[i] = std::lower_bound(s.data[k].begin(), s.data[k].end(), lookup) - s.data[k].begin();
    }
}

void test_t0(State &s) noexcept
{
    // Enter your answer No.0 here.
    // We will track the last position where we found a result for each array.
    std::vector<int> last_position(s.data.size(), 0);

    for (uint32_t i = 0, qn = s.queries.size(); i != qn; ++i)
    {
        const auto [k, lookup] = s.queries[i];

        // Start the search from the last position for this array
        last_position[k] = std::lower_bound(s.data[k].begin() + last_position[k], s.data[k].end(), lookup) - s.data[k].begin();

        // Store the result
        s.answers[i] = last_position[k];
    }
}

void test_t1(State &s) noexcept
{
    // [Optional] Enter your answer No.1 here.
}

void test_t2(State &s) noexcept
{
    // [Optional] Enter your answer No.2 here.
}

void test_t3(State &s) noexcept
{
    // [Optional] Enter your answer No.3 here.
}

} // namespace

int main(int argc, const char **argv)
{
    if (argc != 3)
        return usage();

    test_function_type test_fn = nullptr;

    using namespace std::string_view_literals;
    if (argv[1] == "naive"sv)
        test_fn = &test_naive;
    else if (argv[1] == "t0"sv)
        test_fn = &test_t0;
    else if (argv[1] == "t1"sv)
        test_fn = &test_t1;
    else if (argv[1] == "t2"sv)
        test_fn = &test_t2;
    else if (argv[1] == "t3"sv)
        test_fn = &test_t3;
    else
        return usage("invalid algorithm name\n\n");

    test_param param;
    if (argv[2] == "8"sv)
        param = test_param{
            .data_batch_size = uint32_t(1e6),
            .data_size = 8,
            .query_batch_size = uint32_t(2e6),
            .need_validate = true,
        };
    else if (argv[2] == "big"sv)
        param = test_param{
            .data_batch_size = 500u,
            .data_size = uint32_t(1e6),
            .query_batch_size = 2000u,
            .need_validate = true,
        };
    else
        return usage("invalid size\n\n");

    State s(param.data_batch_size, param.data_size, param.query_batch_size, param.need_validate);

    s.start(argv[1]);
    (*test_fn)(s);
    s.stop();

    return 0;
}
