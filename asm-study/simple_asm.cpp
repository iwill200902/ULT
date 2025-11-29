#include <iostream>


int main() {
    int a = 5, b = 10, c;
    asm volatile (
        "add %[val1], %[val2]\n\t"
        "mov %[val2], %[result]\n\t"
        : [result] "=r" (c)
        : [val1] "r" (a), [val2] "r" (b)
        : "cc"
    );
    std::cout << "Result: " << c << std::endl;
    return 0;
}