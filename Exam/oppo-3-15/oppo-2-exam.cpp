#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int findMex(unordered_set<int>& set) {
    int mex = 0;
    while (set.find(mex) != set.end()) {
        ++mex;
    }
    return mex;
}

int main()
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    unordered_set<int> set(a.begin(), a.end());
    unordered_map<int, int> cnt;
    for (auto e : a) {
        ++cnt[e];
    }

    int g_mex = findMex(set);

    unordered_map<int, int> mex_map;    // map[i]代表i的mex

    for (int i = 0; i < n; ++i) {
        if (mex_map.find(a[i]) == mex_map.end()) {
            // 还没有记录
            if (cnt[a[i]] > 1) {
                mex_map[a[i]] = g_mex;
                cout << g_mex << ' ';
            } else {
                // 只有一个
                set.erase(a[i]);
                int l_mex = findMex(set);
                cout << l_mex << ' ';
                set.insert(a[i]);
                mex_map[a[i]] = l_mex;
            }
        } else {
            cout << mex_map[a[i]] << ' ';
        }
    }
    cout << endl;

    return 0;
}