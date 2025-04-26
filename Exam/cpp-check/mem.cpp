void stack_overflow() {
    int arr[5];
    arr[10] = 42;  // 越界访问（在栈上）
}

int main() {
    stack_overflow();
    return 0;
}


// valgrind --tool=memcheck --leak-check=full ./main
// g++ -fsanitize=address -g main.cpp -o main