#include <stdio.h>
#include <sys/socket.h>

// gcc hello.c -o hello
// ./hello

int main() {
    int a = 999;
    // %zu is the format specifier for type size_t
    // size_t is an unsigned (wouldn't make sense to be negative) integer type used to represent the size of objects in bytes

    printf("%zu\n", sizeof a); // Prints 4 on my system
    printf("%zu\n", sizeof(2 + 7)); // Prints 4 on my system
    printf("%zu\n", sizeof 3.14); // Prints 8 on my system
}