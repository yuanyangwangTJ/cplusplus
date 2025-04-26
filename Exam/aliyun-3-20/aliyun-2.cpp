#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;
using VVT = vector<vector<int>>;

void UpMove(VVT& nums, int M, int N) {
    for (int i = 0; i < N; ++i) {
        int bound = 0;  // 表示合并界限
        stack<int> stk;
        if (nums[i][0] != 0) {
            stk.push(nums[i][0]);
        }
        
        for (int j = 1; j < M; ++j) {
            if (!stk.empty()) {
                if (bound) {
                    
                }
                

            } else {
                stk.push(nums[i][j]);
                bound = 0;
            }

        }
    }
}

void oneOps() {
    int C;
    cin >> C;
    vector<int> ops(C);
    for (int i = 0; i < C; ++i) cin >>ops[i];
    int M, N;
    cin >> M >> N;
    vector<vector<int>> nums(M, vector<int>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> nums[i][j];
        }
    }

    for (auto op : ops) {
        switch (op) {
            case 0:

        
        }
    }
    
}


int main()
{
    int T;
    cin >> T;
    while (T--) {

    }

    return 0;
}