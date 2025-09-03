#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using i64 = long long;
#define endl '\n'

constexpr i64 Mod = 1000000007;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void solve() {
    int n;
    cin >> n;

    vector<i64> g(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> g[i];
    }

    vector<vector<i64>> dp(n + 1, vector<i64> (1 << 4, -1));
    dp[0][0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < 4; j++) {
            if (i < j + 1) {
                continue;
            }
            for (int k = 0; k < (1 << 4); k++) {
                if (!(k & (1 << j)) && dp[i - (j + 1)][k] >= 0) {
                    dp[i][k | (1 << j)] = max(dp[i][k | (1 << j)], dp[i - (j + 1)][k] + g[i]);

                } else if (k == (1 << 4) - 1 && dp[i - (j + 1)][k] >= 0) {
                    dp[i][1 << j] = max(dp[i][1 << j], dp[i - (j + 1)][k] + g[i]);
                }
            }
        }
    }

    i64 ans = -1;
    for (int i = 0; i < (1 << 4); i++) {
        ans = max(ans, dp[n][i]);
    }

    cout << ans << endl;

}
 
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); 

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
 
    return 0; 
}   
