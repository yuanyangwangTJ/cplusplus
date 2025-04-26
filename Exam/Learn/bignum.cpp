#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class BigNum {
private:
    string value;  // 存储大整数（仅包含数字，不带符号）
    bool isNegative; // 记录正负号，true 表示负数

    // 辅助函数：去除前导零
    void removeLeadingZeros() {
        while (value.size() > 1 && value[0] == '0') {
            value.erase(value.begin());
        }
        if (value == "0") isNegative = false; // 结果是 0 时，强制设为正数
    }

public:
    // 默认构造函数
    BigNum() : value("0"), isNegative(false) {}

    // 从字符串构造
    BigNum(string str) {
        if (str.empty()) {
            value = "0";
            isNegative = false;
            return;
        }
        
        if (str[0] == '-') {
            isNegative = true;
            str = str.substr(1); // 去掉负号
        } else {
            isNegative = false;
        }
        value = str;
        removeLeadingZeros();
    }

    // 重载输出运算符
    friend ostream& operator<<(ostream& out, const BigNum& num) {
        if (num.isNegative && num.value != "0") out << "-";
        out << num.value;
        return out;
    }

    // 重载输入运算符
    friend istream& operator>>(istream& in, BigNum& num) {
        string input;
        in >> input;
        num = BigNum(input);
        return in;
    }

    // **加法运算**
    BigNum operator+(const BigNum& other) const {
        if (isNegative == other.isNegative) {
            return BigNum(isNegative ? "-" + add(value, other.value) : add(value, other.value));
        } else {
            if (absGreater(value, other.value)) {
                return BigNum(isNegative ? "-" + subtract(value, other.value) : subtract(value, other.value));
            } else {
                return BigNum(other.isNegative ? "-" + subtract(other.value, value) : subtract(other.value, value));
            }
        }
    }

    // **减法运算**
    BigNum operator-(const BigNum& other) const {
        if (isNegative != other.isNegative) {
            return BigNum(isNegative ? "-" + add(value, other.value) : add(value, other.value));
        } else {
            if (absGreater(value, other.value)) {
                return BigNum(isNegative ? "-" + subtract(value, other.value) : subtract(value, other.value));
            } else {
                return BigNum(isNegative ? subtract(other.value, value) : "-" + subtract(other.value, value));
            }
        }
    }

private:
    // **大数加法（无符号）**
    static string add(string num1, string num2) {
        string result = "";
        int carry = 0;
        int i = num1.size() - 1, j = num2.size() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int digit1 = (i >= 0) ? num1[i] - '0' : 0;
            int digit2 = (j >= 0) ? num2[j] - '0' : 0;
            int sum = digit1 + digit2 + carry;
            carry = sum / 10;
            result += (sum % 10) + '0';
            i--; j--;
        }
        reverse(result.begin(), result.end());
        return result;
    }

    // **大数减法（无符号，假设 num1 >= num2）**
    static string subtract(string num1, string num2) {
        string result = "";
        int borrow = 0;
        int i = num1.size() - 1, j = num2.size() - 1;

        while (i >= 0) {
            int digit1 = num1[i] - '0';
            int digit2 = (j >= 0) ? num2[j] - '0' : 0;
            int diff = digit1 - digit2 - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result += (diff + '0');
            i--; j--;
        }

        reverse(result.begin(), result.end());
        while (result.size() > 1 && result[0] == '0') result.erase(result.begin());
        return result;
    }

    // **比较两个无符号数的大小**
    static bool absGreater(const string& num1, const string& num2) {
        if (num1.size() != num2.size()) return num1.size() > num2.size();
        return num1 > num2;
    }
};

int main() {
    BigNum a, b;
    cout << "输入两个大整数：" << endl;
    cin >> a >> b;

    cout << "a + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;

    return 0;
}
