#include <iostream>

int foo( float *f, int *i ) { 
    *i = 1;               
    *f = 0.f;            
   
   return *i;
}

int main() {
    int x = 0;
    
    std::cout << x << "\n";   // Expect 0，Output 0
    x = foo((float*)(&x), &x);
    std::cout << x << "\n";   // Expect 0, But Output 1
    return 0;
}