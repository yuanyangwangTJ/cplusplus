#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;
using VVD = vector<vector<double>>;
using Shocks = pair<int, double>;   // 股票号和股数


int main()
{
    int N, M, K;
    cin >> N >> M >> K;
    vector<vector<double>> prices(N+1, vector<double>(M+1));
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            cin >> prices[i][j];
        }
    }

    VVD dp(N+1, vector<double>(M+1));
    // dp(i,j)表示第i天持股j时的最大金额，0 表示没有持股
    for (int i = 0; i <= M; ++i) {
        dp[1][i] = K;
    }

    vector<std::pair<int, int>> ops;
    // cout << -1 << " ";  // 第一天无需卖出
    int pre_idx = 0;
    for (int i = 2; i <= N; ++i) {
        // 首先找出最大金额
        int stk_idx = 0;
        double max_money = dp[i-1][0];
        for (int k = 1; k <= M; ++k) {
            double num = dp[i-1][k] / prices[i-1][k];
            double today_val = num * prices[i][k];
            if (today_val > max_money) {
                max_money = today_val;
                stk_idx = k;
            }
        }

        if (pre_idx == stk_idx) {
            ops.emplace_back(-1, -1);
        } else {
            ops.emplace_back(pre_idx-1, stk_idx-1);
        }
        
        pre_idx = stk_idx;
        // cout << stk_idx - 1 << endl;    // 前一天需要买入的
        for (int j = 0; j <= M; ++j) {
            // cout << stk_idx << endl;
            dp[i][j] = max_money;
        }
    }
    ops.emplace_back(pre_idx - 1, -1);
    cout << std::fixed << std::setprecision(4) << dp[N][M] << endl;
    for (auto& p : ops) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}