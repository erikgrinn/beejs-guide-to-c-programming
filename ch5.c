#include <stdio.h>

// int main()
// {
//     // %zu is the format specifier for type size_t

//     printf("an int uses %zu bytes of memory\n", sizeof(int));

//     // That prints "4" for me, but can vary by system.
//     // Memory Fun Facts: When you have a data type (like your typical int) that uses more than a byte of memory,
//     // the bytes that make up the data are always adjacent to one another in memory. Sometimes they’re in the order that you expect,
//     // and sometimes they’re not. This is Endianness. While C doesn’t guarantee any particular memory order (it’s platform-dependent),
//     // it’s still generally possible to write code in a way that’s platform-independent where you don’t have to even consider
//     // these pesky byte orderings.
// }

// *a pointer is a variable that holds an address*
// recall that an address is specified by the byte in memory
// each unique address corresponds to one single byte (8 bits) of storage.

// This means that all these things mean the same thing, i.e. a number that represents a point in memory:

// Index into memory (if you’re thinking of memory like a big array)
// Address
// Location
// I’m going to use these interchangeably. And yes, I just threw location in there because you can never have enough words that mean the same thing.

// And a pointer variable holds that address number. Just like a float variable might hold 3.14159.

// Imagine you have a bunch of Post-it® notes all numbered in sequence with their address. (The first one is at index numbered 0, the next at index 1, and so on.)

// In addition to the number representing their positions, you can also write another number of your choice on each. It could be the number of dogs you have. Or the number of moons around Mars…

// …Or, it could be the index of another Post-it note!

// If you have written the number of dogs you have, that’s just a regular variable. But if you wrote the index of another Post-it in there, that’s a pointer. It points to the other note!

// Another analogy might be with house addresses. You can have a house with certain qualities, yard, metal roof, solar, etc. Or you could have the address of that house. The address isn’t the same as the house itself. One’s a full-blown house, and the other is just a few lines of text. But the address of the house is a pointer to that house. It’s not the house itself, but it tells you where to find it.

// And we can do the same thing in the computer with data. You can have a data variable that’s holding some value. And that value is in memory at some address. And you could have a different pointer variable hold the address of that data variable.

// It’s not the data variable itself, but, like with a house address, it tells us where to find it.

// When we have that, we say we have a “pointer to” that data. And we can follow the pointer to access the data itself.

// (Though it doesn’t seem particularly useful yet, this all becomes indispensable when used with function calls. Bear with me until we get there.)

// So if we have an int, say, and we want a pointer to it, what we want is some way to get the address of that int, right? After all, the pointer just holds the address of the data. What operator do you suppose we’d use to find the address of the int?

// Well, by a shocking surprise that must come as something of a shock to you, gentle reader, we use the address-of operator (which happens to be an ampersand: “&”)to find the address of the data. Ampersand.

int main(void)
{

    // 5.1
    int i = 10;

    printf("The value of i is %d\n", i);
    printf("And its address is %p\n", (void *)&i);
    // The above code contains a cast where we coerce the type of the expression &i to be type void*. This is to keep the compiler from throwing a warning here. This is all stuff we haven’t covered yet, so just ignore the (void*) in the code above for now and pretend it’s not there.
    // prints 0x77ffed7afa344 or similar large hex number corressponding to terabytes of data - this is virtual memory/virtual address
    // from virtual memory wiki there is a familiar diagram in-line with gios notes (virtual memory per process, arrows to physical memory as well as disk),
    // with caption: Virtual memory combines active RAM and inactive memory on DASD[a] to form a large range of contiguous addresses.

    // 5.2
    //  int j;  // j's type is "int"
    //  int *p; // p's type is "pointer to an int", or "int-pointer"

    int j;
    int *p; // p is a pointer, but is uninitialized and points to garbage

    p = &j; // p is assigned the address of i--p now "points to" i
}