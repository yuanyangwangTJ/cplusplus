#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const string& foo(const string& a, const string& b) {
    return a.empty() ? b : a;
}

int main()
{
    auto& s = foo("", "foo");
    cout << s << endl;

    return 0;
}
