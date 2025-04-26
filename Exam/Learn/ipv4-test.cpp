#include <iostream>
#include <regex>
#include <string>

bool isValidIPv4(const std::string& ip) {
    std::regex ipv4_pattern(R"(^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\."
                             R"(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\."
                             R"(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\."
                             R"(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])$)");
    return std::regex_match(ip, ipv4_pattern);
}

int main() {
    std::string ip;
    std::cout << "Enter IPv4 address: ";
    std::cin >> ip;

    if (isValidIPv4(ip)) {
        std::cout << ip << " is a valid IPv4 address.\n";
    } else {
        std::cout << ip << " is NOT a valid IPv4 address.\n";
    }
    return 0;
}
