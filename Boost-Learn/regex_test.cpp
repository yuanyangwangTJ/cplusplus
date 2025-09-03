#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main() {
    std::string text = "hello world";
    boost::regex pattern("hello world");

    if (boost::regex_match(text, pattern)) {
        std::cout << "Match found!" << std::endl;
    } else {
        std::cout << "No match." << std::endl;
    }
    return 0;
}