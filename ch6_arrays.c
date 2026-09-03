#include <stdio.h>

void foo(int x[12]);

int main(void)
{
    // 6.1
    int i;
    float f[4]; // Declare an array of 4 floats

    f[0] = 3.14159; // Indexing starts at 0, of course.
    f[1] = 1.41421;
    f[2] = 1.61803;
    f[3] = 2.71828;

    // Print them all out:

    for (i = 0; i < 4; i++)
    {
        printf("%f\n", f[i]);
    }

    // 6.2
    //  no direct way to get length of array
    int x[12]; // 12 ints

    printf("%zu\n", sizeof x);    // 48 total bytes
    printf("%zu\n", sizeof(int)); // 4 bytes per int

    printf("%zu\n", sizeof x / sizeof(int)); // 48/4 = 12 ints!

    // If it’s an array of chars, then sizeof the array is the number of elements,
    // since sizeof(char) is defined to be 1. For anything else, you have to divide by the size of each element.

    // see foo() below outside of main - gets passed as pointer to first element
    foo(x);

    // One more thing you can do with sizeof and arrays is get the size of an array of a fixed number of elements without declaring the array.
    // This is like how you can get the size of an int with sizeof(int).
    // For example, to see how many bytes would be needed for an array of 48 doubles, you can do this:

    sizeof(double[48]);
}

// 6.2
//  But this trick only works in the scope in which the array was defined.If you pass the array to a function,
//  it doesn’t work.Even if you make it “big” in the function signature :

void foo(int x[12])
{
    printf("%zu\n", sizeof x);    // 8?! What happened to 48?
    printf("%zu\n", sizeof(int)); // 4 bytes per int

    printf("%zu\n", sizeof x / sizeof(int)); // 8/4 = 2 ints?? WRONG.
}
// This is because when you “pass” arrays to functions, you’re only passing a pointer to the first element,
// and that’s what sizeof measures.More on this in the Passing Single Dimensional Arrays to Functions section, below.