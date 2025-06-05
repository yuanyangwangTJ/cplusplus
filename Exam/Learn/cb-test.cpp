#include <iostream>
#include <string>
#include <functional>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <mutex>

class AsyncPubSubSystem {
public:
    using Callback = std::function<void(const std::string&)>;

    // 添加可用的 topic（模拟可订阅的主题）
    void addAvailableTopic(const std::string& topic) {
        std::lock_guard<std::mutex> lock(mutex_);
        availableTopics.insert(topic);
    }

    // 异步订阅
    void subscribe(
        const std::string& topic,
        Callback onSuccess,
        Callback onFailure
    ) {
        std::cout << "[System] Subscribing to topic: " << topic << " (async)..." << std::endl;

        // 启动一个线程来模拟异步订阅
        std::thread([this, topic, onSuccess, onFailure]() {
            // 模拟网络延迟或处理耗时
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (availableTopics.find(topic) != availableTopics.end()) {
                    subscribers.insert(topic);
                    if (onSuccess) onSuccess(topic);
                } else {
                    if (onFailure) onFailure(topic);
                }
            }
        }).detach(); // 分离线程，独立运行
    }

private:
    std::unordered_set<std::string> availableTopics;
    std::unordered_set<std::string> subscribers;
    std::mutex mutex_;
};

int main() {
    AsyncPubSubSystem pubsub;

    // 设置可用 topic
    pubsub.addAvailableTopic("weather");
    pubsub.addAvailableTopic("sports");

    // 订阅成功的主题
    pubsub.subscribe("weather",
        [](const std::string& topic) {
            std::cout << "[Success] Subscribed to: " << topic << std::endl;
        },
        [](const std::string& topic) {
            std::cout << "[Failure] Failed to subscribe to: " << topic << std::endl;
        });

    // 订阅不存在的主题
    pubsub.subscribe("finance",
        [](const std::string& topic) {
            std::cout << "[Success] Subscribed to: " << topic << std::endl;
        },
        [](const std::string& topic) {
            std::cout << "[Failure] Failed to subscribe to: " << topic << std::endl;
        });

    // 为了等待异步线程完成（实际应用中应更优雅地同步）
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}