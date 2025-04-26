#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<pair<int, int>> parseArray(const string& array_str) {
    vector<pair<int, int>> result;
    stringstream ss(array_str);
    char ch; // 用于丢弃无用字符 '[', ']', ',', '(', ')'
    int num, count;

    while (ss >> ch) {
        if (isdigit(ch) || ch == '-' || ch == '+') { // 读取数字
            ss.unget(); // 将读取的数字字符放回流中
            ss >> num;
            if (!(ss >> ch && ch == '(')) { // 如果下一个字符不是 '('，则表示次数为1
                ss.unget(); // 将读取的字符放回流中
                count = 1;
            } else { // 否则读取括号中的数字
                ss >> count;
                ss >> ch; // 读取 ')'
            }
            result.push_back(make_pair(num, count));
        }
    }

    return result;
}

vector<pair<int, int>> mergeAdjacentPairs(const vector<pair<int, int>>& pairs) {
    vector<pair<int, int>> mergedPairs;

    if (pairs.empty()) {
        return mergedPairs;
    }

    mergedPairs.push_back(pairs[0]);

    for (size_t i = 1; i < pairs.size(); ++i) {
        if (pairs[i].first == mergedPairs.back().first) {
            mergedPairs.back().second += pairs[i].second;
        } else {
            mergedPairs.push_back(pairs[i]);
        }
    }

    return mergedPairs;
}

string formatCompressedArray(const vector<pair<int, int>>& compressedArray) {
    stringstream ss;
    ss << '[';
    for (size_t i = 0; i < compressedArray.size(); ++i) {
        if (i != 0) {
            ss << ',';
        }
        ss << compressedArray[i].first << '(' << compressedArray[i].second << ')';
    }
    ss << ']';
    return ss.str();
}

int main() {
    string input;
    cin >> input;

    auto res = parseArray(input);
    auto ret = mergeAdjacentPairs(res);
    cout << formatCompressedArray(ret) << endl;

    return 0;
}
