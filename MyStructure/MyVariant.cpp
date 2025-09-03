#include <type_traits>
#include <iostream>
#include <variant>

template <typename... Args>
struct MaxSizeTraits {
    ;
};

template <typename T, typename... Args>
struct MaxSizeTraits<T, Args...> {
    static constexpr size_t size = std::max(sizeof(T), MaxSizeTraits<Args...>::size);
};

template <>
struct MaxSizeTraits<> {
    static constexpr size_t size = 0;
};

template <size_t index, typename T, typename... Args>
struct IndexTypeTraits {
    static constexpr size_t len = sizeof...(Args) + 1;
    static_assert(index < len);
    using type = typename IndexTypeTraits<index - 1, Args...>::type;
};

template <typename T, typename... Args>
struct IndexTypeTraits<0, T, Args...> {
    using type = T;
};

// 比较T和Args，得到T在Args中的序号
template <typename T, typename... Args>
struct TypeIndexTraits {
    ;
};

template <typename T, typename First, typename... Args>
struct TypeIndexTraits<T, First, Args...> {
    static constexpr int index = std::is_same_v<T, First> ? 0 : 1 + TypeIndexTraits<T, Args...>::index;
};

template <typename T>
struct TypeIndexTraits<T> {
    static constexpr int index = 0;
};

template <typename... Args>
struct Variant {

    template <size_t i>
    auto& get() {
        using Type = typename IndexTypeTraits<i, Args...>::type;
        return *((Type*)m_data);
    }

    constexpr int index() const {
        return m_index;
    }

    constexpr bool valueless_by_exception() const {
        return m_valueless;
    }

    template <typename T>
    constexpr Variant& operator=(const T& t) {
        T* ptr = (T*)m_data;
        if (!valueless_by_exception()) {
            ptr->~T();
        }
        ptr = new (m_data) T(t);
        m_valueless = false;
        m_index = TypeIndexTraits<T, Args...>::index;
        return *this;
    }

    template <typename T>
    constexpr Variant& operator=(T&& t) {
        T* ptr = (T*)m_data;
        if (!valueless_by_exception()) {
            ptr->~T();
        }
        ptr = new (m_data) T(std::forward<T>(t));
        m_valueless = false;
        m_index = TypeIndexTraits<T, Args...>::index;
        return *this;
    }

private:
    static constexpr size_t max_size = MaxSizeTraits<Args...>::size;
    uint8_t m_data[max_size];
    int m_index{0};
    bool m_valueless{true};
};

template <size_t idx>
struct VisitTraits {
    template <typename Vistor>
    static void impl(Vistor&& vistor, int index, auto& var) {
        if (index == idx) {
            vistor(var. template get<idx>());
        } else {
            VisitTraits<idx - 1>::impl(std::forward<Vistor>(vistor), index, var);
        }
    }
};

template <>
struct VisitTraits<0> {
    template <typename Vistor>
    static void impl(Vistor&& vistor, int index, auto& var) {
        if (index == 0) {
            vistor(var. template get<0>());
        } else {
            throw std::bad_variant_access();
        }
    }
};

template <typename Vistor, typename... Types>
auto visit(Vistor&& vistor, Variant<Types...> var) {
    static constexpr int len = sizeof...(Types);

    int index = var.index();
    VisitTraits<len - 1>::impl(std::forward<Vistor>(vistor), index, var);
}

int main() {
    Variant<int, double> var;
    var = 1;
    visit([](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            printf("int, %d\n", val);
        } else if constexpr (std::is_same_v<T, double>) {
            printf("double, %lf\n", val);
        }
    }, var);

    var = 1.2;
    visit([](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            printf("int, %d\n", val);
        } else if constexpr (std::is_same_v<T, double>) {
            printf("double, %lf\n", val);
        }
    }, var);
}