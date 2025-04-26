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

int main()
{
    ll A, B, C, D, N;
    cin >> A >> B >> C >> D >> N;
    bool flag = false;
    for (int x = 0; x <= 2500 && x <= N / A; ++x) {
        ll xs = x * A;
        
        for (int y = 0; y <= 2500 && y <= (N - xs)/B; ++y) {
            ll ys = xs + y * B;
            for (int z = 0; z <= 2500 && z <= (N-ys)/C; ++z) {
                ll zs = ys + z * C;
                ll ws = N - zs;
                if (ws < 0) break;
                if (ws % D == 0 && ws / D >= 0 && ws / D <= 2500) {
                    flag = true;
                    cout << x << " " << y << " " << z << " " << ws/D << endl;
                    return 0;
                }
            }
        }
    }
    cout << -1 << endl;

    return 0;
}