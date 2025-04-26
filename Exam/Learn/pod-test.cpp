#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

struct PODType {
    int x;
    double y;
    void fun(){}
    PODType(){}
};

class NonPODType {
public:
    std::string s;
};

struct Trivial2
{
   int i;
   Trivial2(int a, int b) : i(a), j(b) {}
   Trivial2() = default;
private:
   int j;   // Different access control
};

struct SL
{
   // All members have same access:
   int i;
   int j;
   SL(int a, int b) : i(a), j(b) {} // User-defined constructor OK
};


int main() {
    std::cout << std::boolalpha;
    // std::cout << "PODType is POD? " << std::is_pod<PODType>::value << std::endl;
    // std::cout << "NonPODType is POD? " << std::is_pod<NonPODType>::value << std::endl;

    cout << std::is_trivial_v<PODType> << endl;
    cout << std::is_standard_layout_v<PODType> << endl;
    cout << std::is_trivial_v<Trivial2> << endl;
    cout << std::is_trivial_v<SL> << endl;
    cout << std::is_standard_layout_v<SL> << endl;

    
    return 0;
}
