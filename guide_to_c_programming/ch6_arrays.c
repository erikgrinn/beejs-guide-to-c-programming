#include <stdio.h>

void foo(int x[12]);
void double_array(int *a, int len);

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

    // 6.2 Getting length of an array
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

    // 6.3 Array initializers
    int j;
    int a[5] = {22, 37, 3490, 18, 95}; // Initialize with these values

    for (i = 0; i < 5; i++)
    {
        printf("%d\n", a[i]);
    }

    // You should never have more items in your initializer than there is room for in the array, or the compiler will get cranky:

    // foo.c: In function ‘main’:
    // foo.c:6:39: warning: excess elements in array initializer
    //     6 |     int a[5] = {22, 37, 3490, 18, 95, 999};
    //       |                                       ^~~
    // foo.c:6:39: note: (near initialization for ‘a’)

    // But (fun fact!) you can have fewer items in your initializer than there is room for in the array. The remaining elements in the array will be automatically initialized with zero. This is true in general for all types of array initializers: if you have an initializer, anything not explicitly set to a value will be set to zero.

    // int a[5] = {22, 37, 3490};

    // // is the same as:

    // int a[5] = {22, 37, 3490, 0, 0};

    // It’s a common shortcut to see this in an initializer when you want to set an entire array to zero:

    // int a[100] = {0};
    // Which means, “Make the first element zero, and then automatically make the rest zero, as well.”

    // Lastly, you can also have C compute the size of the array from the initializer, just by leaving the size off:

    // int a[3] = {22, 37, 3490};

    // // is the same as:

    // int a[] = {22, 37, 3490};  // Left the size off!

    // 6.4 Out of Bounds
    // C doesn’t stop you from accessing arrays out of bounds. It might not even warn you.

    // Let’s steal the example from above and keep printing off the end of the array. It only has 5 elements, but let’s try to print 10 and see what happens:

    // #include <stdio.h>

    // int main(void)
    // {
    //     int i;
    //     int a[5] = {22, 37, 3490, 18, 95};

    //     for (i = 0; i < 10; i++) {  // BAD NEWS: printing too many elements!
    //         printf("%d\n", a[i]);
    //     }
    // }
    // Running it on my computer prints:

    // 22
    // 37
    // 3490
    // 18
    // 95
    // 32765
    // 1847052032
    // 1780534144
    // -56487472
    // 21890
    // Yikes! What’s that? Well, turns out printing off the end of an array results in what C developers call undefined behavior. We’ll talk more about this beast later, but for now it means, “You’ve done something bad, and anything could happen during your program run.”

    // And by anything, I mean typically things like finding zeroes, finding garbage numbers, or crashing. But really the C spec says in this circumstance the compiler is allowed to emit code that does anything58.

    // Short version: don’t do anything that causes undefined behavior. Ever59.

    // 6.5 multidimensional arrays
    // 6.6 Arrays and pointers
    // 6.6.1 Getting a Pointer to an Array
    // I want to tell you a secret. Generally speaking, when a C programmer talks about a pointer to an array, they’re talking about a pointer to the first element of the array61.

    // So let’s get a pointer to the first element of an array.

    int b[] = {11, 22, 33, 44, 55};
    int *p;

    p = &b[0]; // p points to the array
               // Well, to the first element, actually

    printf("%d\n", *p); // Prints "11"

    // This is so common to do in C that the language allows us a shorthand:

    p = &b[0]; // p points to the array

    // is the same as:

    p = b; // p points to the array, but much nicer-looking!

    // 6.6.2 passing to function
    //     All these methods of listing the array as a parameter in the function are identical.

    // void times2(int *a, int len)
    // void times3(int a[], int len)
    // void times4(int a[5], int len)

    // In usage by C regulars, the first is the most common, by far.

    // 6.6.3 Changing Arrays in Functions
    // We’ve said that arrays are just pointers in disguise. This means that if you pass an array to a function, you’re likely passing a pointer to the first element in the array.

    // But if the function has a pointer to the data, it is able to manipulate that data! So changes that a function makes to an array will be visible back out in the caller.

    // Here’s an example where we pass a pointer to an array to a function, the function manipulates the values in that array, and those changes are visible out in the caller.

    int y[5] = {1, 2, 3, 4, 5};

    double_array(y, 5);

    for (int i = 0; i < 5; i++)
        printf("%d\n", y[i]); // 2, 4, 6, 8, 10!

    // Even though we passed the array in as parameter a which is type int*, look at how we access it using array notation with a[i]! Whaaaat. This is totally allowed.

    // Later when we talk about the equivalence between arrays and pointers, we’ll see how this makes a lot more sense. For now, it’s enough to know that functions can make changes to arrays that are visible out in the caller.
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

// 6.6.3
void double_array(int *c, int len)
{
    // Multiply each element by 2
    //
    // This doubles the values in x in main() since x and a both point
    // to the same array in memory!

    for (int i = 0; i < len; i++)
        c[i] *= 2;
}