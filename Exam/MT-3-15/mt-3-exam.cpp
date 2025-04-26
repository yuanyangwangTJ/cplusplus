#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

using VT = vector<int>;
using VVT = vector<VT>;

int weightCompute(int k, int a, int b) {
    if (a == b) return 0;
    int cnt = 0;
    for (int x = 1; x <= k / a; ++x) {
        int y = (k - a*x) / b;
        if ((k - a*x) % b == 0 && y != 0) {
            ++cnt;
        }
    }
    return cnt;
}

void buildGrap(VT& ai, int k, VVT& g) {
    int n = ai.size();
    for (int i = 0; i < n; ++i) {
        for (int v = i+1; v < n; ++v) {
            int cnt = weightCompute(k, ai[i], ai[v]);
            g[i][v] = g[v][i] = cnt;
        }
    }
}

int minTree(VVT& g) {
    int sum = 0;
    int n = g.size();
    vector<bool> visisted(n, false);

    for (int i = 0; i < n; ++i) {
        // if ()
    }


}


int main() {
    int n, k;
    cin >> n >> k;
    VT ai(n);
    for (int i = 0; i < n; ++i) {
        cin >> ai[i];
    }

    VVT grap(n, VT(n, 0));  // 图

    buildGrap(ai, k, grap);
    for (int i = 0; i < n; ++i) {
        for (int v = 0; v < n; ++v) {
            cout << grap[i][v] << " ";
        }
        cout << endl;
    }

    return 0;
}
