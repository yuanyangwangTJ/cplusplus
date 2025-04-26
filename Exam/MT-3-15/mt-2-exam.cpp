#include <iostream>
#include <algorithm>

using namespace std;


int main() {
    int l1, r1, l2, r2;
    cin >> l1 >> r1 >> l2 >> r2;

    // for (int i = l1; i <= r1; ++i) {

    // }
    int sum = 0;
    for (int i = l2; i <= r2; ++i) {
        int l = l1 / i;
        int r = r1 / i;
        int flag = 0;
        if (l1 % i == 0) {
            flag = 1;
        } 
        // cout << i << " "  << l << " " << r << ' ' << flag << endl;
        sum += (r- l + flag);
    }
    cout << sum <<endl;

    return 0;
}
