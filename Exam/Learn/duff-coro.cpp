#include <iostream>

int coroutine() {
    static int i, state = 0;
    switch (state) {
        case 0:
            for (i = 0; i < 10; i++) {
                state = __LINE__ + 2;
                return i;
                case __LINE__:;
            }
    }
    return -1;
}

int main() {
    ::std::cout << coroutine() << ::std::endl; // 0
    ::std::cout << coroutine() << ::std::endl; // 1
    ::std::cout << coroutine() << ::std::endl; // 2
}