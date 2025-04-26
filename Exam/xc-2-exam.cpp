#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <numeric>

using namespace std;

auto calOperationsToEqualize(const vector<int>& vec) {
    auto array = vec;
    sort(array.begin(), array.end());

    int n = static_cast<int>(vec.size());
    int ops_pre = accumulate(array.begin(), array.end(), 0) - n * array[0];
    unordered_map<int, int> ops_map;
    ops_map[array[0]] = ops_pre;
    for (int i = 1; i < n; ++i) {
        ops_pre += (2*i-n)*(array[i]-array[i-1]);
        ops_map[array[i]] = ops_pre;
    }
    return ops_map;
}

int minOperationsToEqualize(const vector<int>& array, int i) {

    // 计算操作次数
    int operations = 0;
    auto val = array[i];
    for (auto s : array) {
        operations += abs(s - val);
    }

    return operations;
}

int main() {
    int n;
    cin >> n;
    vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    auto ops_map = calOperationsToEqualize(vec);
    // 输出操作次数
    // for (auto e : vec) {
    //     cout << ops_map[e] << ' ';
    // }

    for (int i = 0; i < n; i++) {
        auto res = ops_map[vec[i]];
        cout << res << ' ';
        if (res != minOperationsToEqualize(vec, i)) {
            cout << "error occurs!" << endl;
            return -1;
        }
    }

    cout << endl << "coreect!" << endl;

    return 0;
}
