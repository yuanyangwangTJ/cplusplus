#include <memory>
#include <utility>
#include <iostream>
#include <functional> // For std::bad_function_call

template<typename R, typename... Args>
class MyFunction;

/**
 * @brief MyFunction 的偏特化版本，用于处理函数签名。
 * 
 * @tparam R 返回类型
 * @tparam Args 参数类型
 */
template<typename R, typename... Args>
class MyFunction<R(Args...)> {
private:
    // 抽象基类，用于类型擦除
    struct CallableBase {
        virtual ~CallableBase() = default;
        virtual R invoke(Args... args) = 0;
        virtual std::unique_ptr<CallableBase> clone() const = 0;
    };

    // 派生类模板，用于存储任意类型的可调用对象
    template<typename F>
    struct Callable : public CallableBase {
        F f;

        // 通过值传递和移动来存储可调用对象
        Callable(F functor) : f(std::move(functor)) {}

        R invoke(Args... args) override {
            return f(std::forward<Args>(args)...);
        }

        std::unique_ptr<CallableBase> clone() const override {
            return std::make_unique<Callable<F>>(f);
        }
    };

    std::unique_ptr<CallableBase> m_callable;

public:
    // 默认构造函数，创建一个空的 MyFunction
    MyFunction() noexcept = default;

    // 从 nullptr 构造
    MyFunction(std::nullptr_t) noexcept : m_callable(nullptr) {}

    // 拷贝构造函数
    MyFunction(const MyFunction& other) {
        if (other.m_callable) {
            m_callable = other.m_callable->clone();
        }
    }

    // 移动构造函数
    MyFunction(MyFunction&& other) noexcept = default;

    /**
     * @brief 从任意可调用对象构造 MyFunction。
     * @note 使用 SFINAE 来避免此构造函数匹配 MyFunction 本身。
     */
    template<typename F,
             typename = std::enable_if_t<
                 !std::is_same_v<std::decay_t<F>, MyFunction>
             >>
    MyFunction(F f) : m_callable(std::make_unique<Callable<F>>(std::move(f))) {}

    // 拷贝赋值运算符
    MyFunction& operator=(const MyFunction& other) {
        if (this != &other) {
            MyFunction temp(other);
            std::swap(m_callable, temp.m_callable);
        }
        return *this;
    }

    // 移动赋值运算符
    MyFunction& operator=(MyFunction&& other) noexcept = default;

    // 赋值为 nullptr，清空 MyFunction
    MyFunction& operator=(std::nullptr_t) noexcept {
        m_callable.reset();
        return *this;
    }

    // 调用存储的可调用对象
    R operator()(Args... args) const {
        if (!m_callable) {
            throw std::bad_function_call();
        }
        return m_callable->invoke(std::forward<Args>(args)...);
    }

    // 检查 MyFunction 是否为空
    explicit operator bool() const noexcept {
        return m_callable != nullptr;
    }
};

// --- 使用示例 ---

void free_function(int a) {
    std::cout << "自由函数: " << a << std::endl;
}

struct Functor {
    void operator()(int a) const {
        std::cout << "函数对象: " << a << std::endl;
    }
};

int main() {
    // 存储自由函数
    MyFunction<void(int)> func1 = free_function;
    func1(10);

    // 存储 Lambda 表达式
    MyFunction<void(int)> func2 = [](int a) {
        std::cout << "Lambda: " << a << std::endl;
    };
    func2(20);

    // 存储函数对象
    Functor functor_instance;
    MyFunction<void(int)> func3 = functor_instance;
    func3(30);

    // 测试拷贝构造
    MyFunction<void(int)> func4 = func2;
    func4(40);
    std::cout << "func2 " << (func2 ? "not empty" : "empty") << std::endl;


    // 测试移动构造
    MyFunction<void(int)> func5 = std::move(func1);
    func5(50);
    if (!func1) {
        std::cout << "func1 在移动后为空" << std::endl;
    }
    
    // 测试拷贝赋值
    func1 = func3;
    func1(60);

    // 测试带有返回值的函数
    MyFunction<int(int, int)> add = [](int a, int b) { return a + b; };
    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;

    // 测试清空
    func2 = nullptr;
    if (!func2) {
        std::cout << "func2 在赋值为 nullptr 后为空" << std::endl;
    }

    return 0;
}
