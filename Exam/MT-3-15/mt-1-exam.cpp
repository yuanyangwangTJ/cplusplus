#include <algorithm>
#include <iostream>

using namespace std;

int main() 
{
    int T;

    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        string str;
        for (int i = 0; i < s.size(); ++i) {
            char ch = s[i];
            if (ch == 'R') {
                if (i+1 < s.size() && s[i+1] == 'Z') {
                    ++i;
                    continue;
                }
                std::reverse(str.begin(), str.end());
            } else if (ch == 'Z') {
                if (i-1>=0 && s[i-1] == 'R') {
                    continue;
                } else if (i-1>=0) {
                    str.pop_back();
                }
            } else {
                str += ch;
            }
        }
        cout << str << endl;
    }


    return 0;
}