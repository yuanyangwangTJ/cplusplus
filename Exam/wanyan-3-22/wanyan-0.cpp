#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using ll = long long;
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    ll sum = 0;
    for (int i = 0; i < n; ++i) {
        std::priority_queue<int> left;
        std::priority_queue<int, vector<int>, std::greater<>> right;
        // 保证左边数量 >= 右边
        // 维护左边最大值 <= 右边最小值
        for (int j = i; j < n; ++j) {
            // cout << 1 << endl;
            if (left.empty() || nums[j] <= left.top()) {
                left.push(nums[j]);
            } else {
                right.push(nums[j]);
            }

            while (left.size() > right.size() + 1) {
                // cout << 1 << endl;
                int top = left.top();
                left.pop();
                right.push(top);
            }

            while (right.size() > left.size()) {
                int top = right.top();
                right.pop();
                left.push(top);
            }

            // cout << left.top();
            sum += left.top();
        }
    }

    cout << sum << endl;

    return 0;
}