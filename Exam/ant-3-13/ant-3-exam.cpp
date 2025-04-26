#include <iostream>
#include <vector>
#include <numeric>  // gcd
#include <cstdint>  // int64_t
using namespace std;

// 计算最小公倍数（避免溢出）
int64_t lcm(int64_t a, int64_t b) {
    if (a > 1e18 / b) return 1e18 + 1;  // 溢出保护，返回大于 r 的值
    return a * (b / gcd(a, b));
}

// 计算 [l, r] 区间内 t ≡ 1 (mod a) 的时刻数
int64_t count_multiples(int64_t a, int64_t l, int64_t r) {
    return (r - 1) / a - (l - 2) / a;
}

// 计算 [l, r] 内至少有兔子出洞的时刻数
int64_t count_rabbit_times(const vector<int64_t>& a_list, int64_t l, int64_t r) {
    int n = a_list.size();
    int64_t total = 0;

    // 遍历所有可能的 LCM 组合
    for (int mask = 1; mask < (1 << n); ++mask) {
        int64_t lcm_value = 1;
        int bits = 0;

        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {  // 选中 a_list[i]
                lcm_value = lcm(lcm_value, a_list[i]);
                if (lcm_value > r) break;  // 超出范围，停止计算
                bits++;
            }
        }

        if (lcm_value <= r) {
            int64_t count = count_multiples(lcm_value, l, r);
            if (bits % 2 == 1) total += count;  // 奇数个元素，加
            else total -= count;               // 偶数个元素，减
        }
    }

    return total;
}

int main() {
    int n;
    int64_t l, r;
    cin >> n >> l >> r;
    vector<int64_t> a_list(n);
    for (int i = 0; i < n; i++) {
        cin >> a_list[i];
    }
    
    cout << count_rabbit_times(a_list, l, r) << endl;
    return 0;
}
