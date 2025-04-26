#include <cstdlib>

struct MyStruct {
    int a;
};

int main() {
    void* p = malloc(sizeof(MyStruct));
    MyStruct* obj = static_cast<MyStruct*>(p);  // 
    return 0;
}
