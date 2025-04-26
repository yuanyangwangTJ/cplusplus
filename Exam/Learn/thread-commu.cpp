#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void producer(std::promise<int> p) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(42);
}

void consumer(std::future<int> f) {
    int value = f.get();
    std::cout << "receive value: " << value << std::endl;
}

int main()
{
    std::promise<int> p;
    auto f = p.get_future();

    std::jthread t1(producer, std::move(p));
    std::jthread t2(consumer, std::move(f));

}