#include <iostream>
#include <vector>

using namespace std;

long calculateModeSum(vector<int>& arr) {
    int n = int(arr.size());
    vector<int> prefix1(n + 1, 0);
    vector<int> prefix2(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        prefix1[i] = prefix1[i - 1] + (arr[i - 1] == 1 ? 1 : 0);
        prefix2[i] = prefix2[i - 1] + (arr[i - 1] == 2 ? 1 : 0);
    }

    long totalSum = 0;
    for (int start = 0; start < n; start++) {
        for (int end = start + 1; end <= n;) {
            int count1 = prefix1[end] - prefix1[start];
            int count2 = prefix2[end] - prefix2[start];

            if (count1 == count2) {
                totalSum += 1;
                end++;
            }
            else if (count1 > count2) {
                int diff = count1 - count2;
                if (start + diff + count1 + count2 > n) {
                    totalSum += n - count1 - count2 - start + 1;
                    break;
                } else {
                    totalSum += diff;
                    end += diff;
                }
            } else {
                int diff = count2 - count1;
                if (start + diff + count1 + count2 > n) {
                    totalSum += (n - count1 - count2 - start + 1) * 2;
                    break;
                } else {
                    totalSum += diff * 2;
                    end += diff;
                }
            }
        }
    }

    return totalSum;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << calculateModeSum(arr) << endl;

    return 0;
}
