#include <cstdlib>
#include <iostream>

using namespace std;

class inner {
   public:
    inner() { cout << "Constructing" << endl; }
    ~inner() { cout << "Destructing" << endl; }
};
 
int main(int argc, char *argv[]) {
    // inner *p = new inner();
    inner *p = (inner*)malloc(sizeof(inner) * 10);
    // delete p;
    delete []p;
    // free(p);
    return 0;
}