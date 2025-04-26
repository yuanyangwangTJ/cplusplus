#include <expected>
#include <iostream>
#include <string>

std::expected<int, std::string> parseNumber(const std::string& str) {
    try {
        return std::stoi(str);  // 成功返回 int
    } catch (...) {
        return std::unexpected("Invalid number format");  // 失败返回错误信息
    }
}

int main() {
    auto result = parseNumber("123");
    if (result) {
        std::cout << "Parsed number: " << *result << '\n';
    } else {
        std::cout << "Error: " << result.error() << '\n';
    }
}
