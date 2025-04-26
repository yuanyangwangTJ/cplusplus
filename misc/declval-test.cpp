#include <utility>

class Fraction {
public:
    Fraction(int n, int d): num_(n), den_(d) {}
    Fraction operator+(const Fraction& rhs) const {
        return Fraction(num_ * rhs.den_ + den_ * rhs.num_, den_ * rhs.den_);
    }
    // 其他代码
    Fraction operator-() const {
        return Fraction(-num_, -den_);
    }

    Fraction operator-(const Fraction& rhs) const {
        return *this + (-rhs);
    }

    Fraction operator*(const Fraction& rhs) const {
        return Fraction(num_ * rhs.num_, den_ * rhs.den_);
    }

    // 加法、减法、乘法的重载（略）
    Fraction operator+(int rhs) const {
        return Fraction(num_ + den_ * rhs, den_);
    }

    Fraction operator-(int rhs) const {
        // 这里因为类型嵌套的问题，需要使用 decltype & 或 decltype(declval())
        return *this - Fraction(std::declval<int>(), rhs);
    }

    Fraction operator*(int rhs) const {
        return Fraction(num_ * rhs, den_);
    }

private:
    int num_, den_;
};

int main()
{
    Fraction f(1, 0);
    return 0;
}
