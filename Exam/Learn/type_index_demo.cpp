#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void derived_func() { std::cout << "Derived function" << std::endl; }
};

// 类型注册系统示例
class TypeRegistry {
private:
    std::map<std::type_index, std::string> type_names;
    std::unordered_map<std::type_index, std::function<void()>> type_factories;
    
public:
    template<typename T>
    void register_type(const std::string& name) {
        type_names[std::type_index(typeid(T))] = name;
        type_factories[std::type_index(typeid(T))] = []() {
            std::cout << "Creating object of type: " << typeid(T).name() << std::endl;
        };
    }
    
    void print_registered_types() {
        std::cout << "已注册的类型:" << std::endl;
        for (const auto& pair : type_names) {
            std::cout << "  " << pair.second << " -> " << pair.first.name() << std::endl;
        }
    }
    
    template<typename T>
    void create_object() {
        std::type_index index(typeid(T));
        auto it = type_factories.find(index);
        if (it != type_factories.end()) {
            it->second();
        } else {
            std::cout << "类型未注册: " << typeid(T).name() << std::endl;
        }
    }
};

int main() {
    std::cout << "=== type_index 基本概念 ===" << std::endl;
    
    // 1. 基本使用
    std::type_index int_index(typeid(int));
    std::type_index double_index(typeid(double));
    std::type_index string_index(typeid(std::string));

    
    std::cout << "int的type_index: " << int_index.name() << std::endl;
    std::cout << "double的type_index: " << double_index.name() << std::endl;
    std::cout << "string的type_index: " << string_index.name() << std::endl;
    
    std::cout << "\n=== type_index 作为容器键 ===" << std::endl;
    
    // 2. 使用type_index作为map的键
    std::map<std::type_index, std::string> type_map;
    type_map[std::type_index(typeid(int))] = "整数类型";
    type_map[std::type_index(typeid(double))] = "浮点类型";
    type_map[std::type_index(typeid(std::string))] = "字符串类型";
    
    std::cout << "类型映射:" << std::endl;
    for (const auto& pair : type_map) {
        std::cout << "  " << pair.first.name() << " -> " << pair.second << std::endl;
    }
    
    std::cout << "\n=== type_index 与 type_info 的区别 ===" << std::endl;
    
    // 3. type_info无法直接作为键（以下代码会编译错误）
    // std::map<std::type_info, std::string> error_map;  // 编译错误！
    
    // 但type_index可以
    std::cout << "type_index可以拷贝和比较:" << std::endl;
    std::type_index copy_index = int_index;  // 可以拷贝
    std::cout << "拷贝是否相等: " << (copy_index == int_index) << std::endl;
    std::cout << "不同类型比较: " << (int_index < double_index) << std::endl;
    
    std::cout << "\n=== hash_code 比较 ===" << std::endl;
    
    // 4. hash_code的使用
    std::cout << "type_info hash_code vs type_index hash_code:" << std::endl;
    std::cout << "typeid(int).hash_code(): " << typeid(int).hash_code() << std::endl;
    std::cout << "type_index(typeid(int)).hash_code(): " << int_index.hash_code() << std::endl;
    
    std::cout << "\n=== 实际应用：类型注册系统 ===" << std::endl;
    
    // 5. 实际应用示例
    TypeRegistry registry;
    registry.register_type<int>("整数");
    registry.register_type<double>("浮点数");
    registry.register_type<std::string>("字符串");
    registry.register_type<Base>("基类");
    registry.register_type<Derived>("派生类");
    
    registry.print_registered_types();
    
    std::cout << "\n创建对象:" << std::endl;
    registry.create_object<int>();
    registry.create_object<std::string>();
    registry.create_object<Base>();
    
    std::cout << "\n=== 多态对象的type_index ===" << std::endl;
    
    // 6. 多态对象
    Base* base_ptr = new Derived();
    std::type_index static_type(typeid(Base));
    std::type_index dynamic_type(typeid(*base_ptr));
    
    std::cout << "静态类型: " << static_type.name() << std::endl;
    std::cout << "动态类型: " << dynamic_type.name() << std::endl;
    std::cout << "类型是否相同: " << (static_type == dynamic_type) << std::endl;
    
    std::cout << "\n=== unordered_map 中的使用 ===" << std::endl;
    
    // 7. 在unordered_map中使用
    std::unordered_map<std::type_index, int> type_counter;
    type_counter[std::type_index(typeid(int))] = 1;
    type_counter[std::type_index(typeid(double))] = 2;
    type_counter[std::type_index(typeid(std::string))] = 3;
    
    std::cout << "类型计数器:" << std::endl;
    for (const auto& pair : type_counter) {
        std::cout << "  " << pair.first.name() << " -> " << pair.second << std::endl;
    }
    
    delete base_ptr;
    return 0;
} 