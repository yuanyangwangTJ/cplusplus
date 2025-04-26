#include <iostream>
#include <random>
#include <vector>

using namespace std;

using ll = long long;
constexpr ll MOD = 1000000007;

int cntNums(string& num) {
    vector<ll> dp(3, 0);   // dp[i]存储的当前除以3余数为i的情况数
    dp[0] = 1;
    for (int i = 0; i < num.size(); ++i) {
        char ch = num[i];
        vector<ll> next_dp(3, 0);

        if (ch == '?') {
            for (int d = 0; d <= 9; ++d) {
                if (i == 0 && d == 0) continue;
                for (int m = 0; m < 3; ++m) {
                    next_dp[(d+m)%3] += dp[m];
                }
            }
        } else {
            int d = ch - '0';
            for (int m = 0; m < 3; ++m) {
                next_dp[(d+m)%3] += dp[m];
            }
        }
        dp = next_dp;
    }
    cout << dp[0] << endl;

    return dp[0] % MOD;
}

int main()
{
    int n;
    cin >> n;
    string s;
    cin >> s;
    cout << cntNums(s) << endl;


    return 0;
}