#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using ll = long long;
using namespace std;

int getMedia(vector<int>& vec) {
    sort(vec.begin(), vec.end());
    // cout << "media: " << vec[vec.size() / 2] << endl;
    return vec[(vec.size()-1)>>1];
}

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
        for (int j = i; j < n; ++j) {
            vector<int> res(nums.begin() + i, nums.begin() + j + 1);
            // for (auto e : res) {
            //     cout << e << ' ';
            // }
            // cout << endl;
            sum += getMedia(res);
        }
    }

    cout << sum << endl;

    return 0;
}