#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using ll = long long;

int main()
{
    int n;
    ll t;
    cin >> n >> t;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    auto sum = std::accumulate(nums.begin(), nums.end(), 0ll);
    int cnt = 0;
    for (auto num : nums) {
        if (sum - 2*num >= 0 && sum - 2*num <= t) {
            ++cnt;
        } 
    }
    
    cout << cnt << endl;
    return 0;
}