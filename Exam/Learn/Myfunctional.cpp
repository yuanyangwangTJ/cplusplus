#include <iostream>
#include <memory>
#include <functional>

// 支持 R(Args...) 类型的函数封装
template<typename R, typename... Args>
class MyFunction {
    // 抽象基类（类型擦除核心）
    struct CallableBase {
        virtual R call(Args... args) = 0;
        virtual ~CallableBase() {}
    };

    // 函数指针封装
    struct FuncPtrCallable : CallableBase {
        using FuncPtr = R(*)(Args...);
        FuncPtr func;
        FuncPtrCallable(FuncPtr f) : func(f) {}
        R call(Args... args) override {
            return func(std::forward<Args>(args)...);
        }
    };

    // 成员函数指针封装（需要绑定对象）
    template<typename T>
    struct MemberFuncCallable : CallableBase {
        using MemFunc = R(T::*)(Args...);
        T* object;
        MemFunc method;

        MemberFuncCallable(T* obj, MemFunc mf) : object(obj), method(mf) {}
        R call(Args... args) override {
            return (object->*method)(std::forward<Args>(args)...);
        }
    };

    // 泛型可调用对象（如 lambda、std::function、仿函数）
    template<typename F>
    struct FunctorCallable : CallableBase {
        F func;
        FunctorCallable(F f) : func(std::move(f)) {}
        R call(Args... args) override {
            return func(std::forward<Args>(args)...);
        }
    };

    std::unique_ptr<CallableBase> callable;

public:
    MyFunction() = default;

    // 构造函数：接受普通函数指针
    MyFunction(R(*f)(Args...)) {
        callable = std::make_unique<FuncPtrCallable>(f);
    }

    // 构造函数：接受 lambda、std::function、仿函数等
    template<typename F>
    MyFunction(F f) {
        callable = std::make_unique<FunctorCallable<F>>(std::move(f));
    }

    // 提供显式绑定成员函数 + 对象的方法
    template<typename T>
    void bind_member(T* obj, R(T::*mf)(Args...)) {
        callable = std::make_unique<MemberFuncCallable<T>>(obj, mf);
    }

    R operator()(Args... args) {
        if (!callable) throw std::bad_function_call();
        return callable->call(std::forward<Args>(args)...);
    }
};

void free_func(int x) {
    std::cout << "free_func: " << x << "\n";
}

class MyClass {
public:
    void member_func(int x) {
        std::cout << "member_func: " << x << "\n";
    }
};

int main() {
    // 普通函数指针
    MyFunction<void, int> f1 = free_func;
    f1(10);

    // Lambda 表达式
    MyFunction<void, int> f2 = [](int x) {
        std::cout << "lambda: " << x << "\n";
    };
    f2(20);

    // 成员函数指针 + 绑定对象
    MyClass obj;
    MyFunction<void, int> f3;
    f3.bind_member(&obj, &MyClass::member_func);
    f3(30);

    return 0;
}