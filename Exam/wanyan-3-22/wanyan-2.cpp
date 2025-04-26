#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;
using ll = long long;

void dfs(vector<ll>& res, vector<int>& nums, std::array<int, 4> steps, int first, ll sum, int step, int total_step) {
    if (step >= total_step) {
        // 超出总部数目，退出
        return;
    }
    if (sum < 0) {
        // cout << "sum = " << sum << endl;
        // cout << "sum < 0" << endl;
        return ;
    }
    if (step + 1 == nums.size()) {
        res.emplace_back(sum);
        return ;
    }

    for (int i = first; i < 4; ++i) {
        swap(steps[first], steps[i]);
        int new_step = step + steps[first];
        // cout <<"new step: " << new_step << endl;
        dfs(res, nums, steps, first+1, sum + nums[new_step], new_step, total_step);
        swap(steps[first], steps[i]);
    }
}

// [1,2,3,4]进行跳跃，所能够获得的最大值
ll tenStepMax(vector<int>& nums, ll start) {
    std::array<int, 4> steps {1, 2, 3, 4};
    vector<ll> res;
    dfs(res, nums, steps, 0, start, -1, 10);
    // cout << res.size() << endl;
    if (res.empty()) {
        // cout << "empty?" << endl;
        return -1;
    }
    auto it = max_element(res.begin(), res.end());
    // cout << *it << endl;
    return *it;
}

ll notTenStepMax(vector<int>& nums, ll start) {
    if (nums.empty()) return start;
    std::array<int, 4> steps {1, 2, 3, 4};
    vector<ll> res;
    dfs(res, nums, steps, 0, start, -1, nums.size());
    // cout << res.size() << endl;
    if (res.empty()) {
        // cout << "empty?" << endl;
        return -1;
    }
    auto it = max_element(res.begin(), res.end());
    // cout << *it << endl;
    return *it;
}
// 11
// -1 2 3 4 -9 -9 -1 3 -1 -1 1

int main()
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    ll start = 0;
    int i;
    for (i = 0; i < n; i += 10) {
        if (i + 10 > n) {
            break;
        }

        vector<int> nums(a.begin()+i, a.begin()+i+10);
        // for (auto e : nums) {
        //     cout << e << " ";
        // }
        // cout << endl;
        ll res = tenStepMax(nums, start);
        if (res == -1) {
            // 无法到达
            cout << -1 << endl;
            return 0;
        }
        start = res;
    }
    // cout << start << endl;
    vector<int> nums(a.begin()+i, a.end());
    ll res = notTenStepMax(nums, start);
    if (res == -1) {
        // 无法到达
        cout << -1 << endl;
        return 0;
    }
    cout << res << endl;


    return 0;
}