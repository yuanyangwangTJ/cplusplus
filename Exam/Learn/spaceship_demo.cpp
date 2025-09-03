#include <iostream>
#include <compare>
#include <string>
#include <vector>
#include <algorithm>

// 自定义类，演示三路比较运算符
class Point {
public:
    int x, y;
    
    Point(int x, int y) : x(x), y(y) {}
    
    // 定义三路比较运算符
    auto operator<=>(const Point& other) const {
        // 首先比较x坐标，如果x相等则比较y坐标
        if (auto cmp = x <=> other.x; cmp != 0) {
            return cmp;
        }
        return y <=> other.y;
    }
    
    // 通常还需要定义相等运算符
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// 更复杂的示例：学生类
class Student {
public:
    std::string name;
    int age;
    double gpa;
    
    Student(const std::string& n, int a, double g) 
        : name(n), age(a), gpa(g) {}
    
    // 使用默认的三路比较运算符
    auto operator<=>(const Student& other) const = default;
    
    // 也可以自定义比较逻辑
    auto compare_by_gpa(const Student& other) const {
        return gpa <=> other.gpa;
    }
};

// 演示不同的比较类别
class CustomComparison {
public:
    int value;
    
    CustomComparison(int v) : value(v) {}
    
    // 返回强排序（strong_ordering）
    std::strong_ordering operator<=>(const CustomComparison& other) const {
        if (value < other.value) return std::strong_ordering::less;
        if (value > other.value) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    bool operator==(const CustomComparison& other) const {
        return value == other.value;
    }
};

int main() {
    std::cout << "=== 三路比较运算符 (<=>) 基础用法 ===" << std::endl;
    
    // 1. 基本类型的使用
    int a = 10, b = 20, c = 10;
    
    std::cout << "基本类型比较:" << std::endl;
    auto result1 = a <=> b;
    if (result1 < 0) {
        std::cout << a << " < " << b << std::endl;
    } else if (result1 > 0) {
        std::cout << a << " > " << b << std::endl;
    } else {
        std::cout << a << " == " << b << std::endl;
    }
    
    auto result2 = a <=> c;
    if (result2 == 0) {
        std::cout << a << " == " << c << std::endl;
    }
    
    std::cout << "\n=== 自定义类的三路比较 ===" << std::endl;
    
    // 2. 自定义Point类
    Point p1(1, 2);
    Point p2(1, 3);
    Point p3(2, 1);
    Point p4(1, 2);
    
    std::cout << "Point比较:" << std::endl;
    
    auto cmp1 = p1 <=> p2;
    if (cmp1 < 0) {
        std::cout << "p1(1,2) < p2(1,3)" << std::endl;
    }
    
    auto cmp2 = p1 <=> p3;
    if (cmp2 < 0) {
        std::cout << "p1(1,2) < p3(2,1)" << std::endl;
    }
    
    auto cmp3 = p1 <=> p4;
    if (cmp3 == 0) {
        std::cout << "p1(1,2) == p4(1,2)" << std::endl;
    }
    
    std::cout << "\n=== 自动生成的比较运算符 ===" << std::endl;
    
    // 3. 三路比较运算符自动生成其他比较运算符
    std::cout << "自动生成的运算符:" << std::endl;
    std::cout << "p1 < p2: " << (p1 < p2) << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2) << std::endl;
    std::cout << "p1 > p2: " << (p1 > p2) << std::endl;
    std::cout << "p1 >= p2: " << (p1 >= p2) << std::endl;
    std::cout << "p1 == p4: " << (p1 == p4) << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2) << std::endl;
    
    std::cout << "\n=== 默认三路比较 ===" << std::endl;
    
    // 4. 使用默认的三路比较
    Student s1("Alice", 20, 3.8);
    Student s2("Bob", 19, 3.9);
    Student s3("Charlie", 20, 3.8);
    
    std::cout << "Student比较（默认按成员顺序）:" << std::endl;
    if (s1 < s2) {
        std::cout << "s1 < s2" << std::endl;
    }
    if (s1 == s3) {
        std::cout << "s1 == s3" << std::endl;
    }
    
    std::cout << "\n=== 容器中的排序 ===" << std::endl;
    
    // 5. 在容器中使用
    std::vector<Point> points = {
        {3, 1}, {1, 3}, {2, 2}, {1, 1}, {2, 1}
    };
    
    std::cout << "排序前:" << std::endl;
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << std::endl;
    
    std::sort(points.begin(), points.end());
    
    std::cout << "排序后:" << std::endl;
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << std::endl;
    
    std::cout << "\n=== 比较类别 ===" << std::endl;
    
    // 6. 不同的比较类别
    CustomComparison c1(10);
    CustomComparison c2(20);
    
    auto strong_result = c1 <=> c2;
    std::cout << "强排序比较结果类型: ";
    if (strong_result < 0) {
        std::cout << "less" << std::endl;
    } else if (strong_result > 0) {
        std::cout << "greater" << std::endl;
    } else {
        std::cout << "equal" << std::endl;
    }
    
    std::cout << "\n=== 字符串比较 ===" << std::endl;
    
    // 7. 字符串的三路比较
    std::string str1 = "apple";
    std::string str2 = "banana";
    std::string str3 = "apple";
    
    std::cout << "字符串比较:" << std::endl;
    auto str_cmp1 = str1 <=> str2;
    if (str_cmp1 < 0) {
        std::cout << "\"" << str1 << "\" < \"" << str2 << "\"" << std::endl;
    }
    
    auto str_cmp2 = str1 <=> str3;
    if (str_cmp2 == 0) {
        std::cout << "\"" << str1 << "\" == \"" << str3 << "\"" << std::endl;
    }
    
    return 0;
} 