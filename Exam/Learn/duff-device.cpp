#include <iostream>
#include <cstring>

void duff_copy(char* to, const char* from, int count) {
    int n = (count + 7) / 8;  // 计算循环次数（每次拷贝 8 字节）
    
    switch (count % 8) {  // 处理剩余的 0~7 个字符
        case 0: do {  *to++ = *from++;
        case 7:       *to++ = *from++;
        case 6:       *to++ = *from++;
        case 5:       *to++ = *from++;
        case 4:       *to++ = *from++;
        case 3:       *to++ = *from++;
        case 2:       *to++ = *from++;
        case 1:       *to++ = *from++;
                } while (--n > 0);
    }
}

int main() {
    char src[] = "Hello, Duff's Device!";
    char dst[50];

    duff_copy(dst, src, strlen(src) + 1);
    std::cout << "Copied String: " << dst << std::endl;
    
    return 0;
}
