#include <stdio.h>

int main(void)
{
    // 7.2 String Variables
    // Now that we know how to make a string literal, let’s assign it to a variable so we can do something with it.

    char *s = "Hello, world!";
    // Check out that type: pointer to a char. The string variable s is actually a pointer to the first character in that string, namely the H.

    // And we can print it with the %s (for “string”) format specifier:

    printf("%s\n", s); // "Hello, world!"

    // 7.3 String Variables as Arrays

    char s1[] = "Hello, world!";

    for (int i = 0; i < 13; i++)
        printf("%c", s1[i]);
    printf("\n");

    // Note that we’re using the format specifier %c to print a single character.

    // Also, check this out. The program will still work fine if we change the definition of s to be a char* type:

    char *s2 = "Hello, world!"; // char* here

    for (int i = 0; i < 13; i++)
        printf("%c", s2[i]); // But still use arrays here...?
    printf("\n");
}
