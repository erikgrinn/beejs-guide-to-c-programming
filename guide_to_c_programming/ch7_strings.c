#include <stdio.h>
#include <string.h>

// Finally! Strings! What could be simpler?

// Well, turns out strings aren’t actually strings in C. That’s right! They’re pointers! Of course they are!

// Much like arrays, strings in C barely exist.

// But let’s check it out—it’s not really such a big deal.

// Before we start, let’s talk about string literals in C. These are sequences of characters in double quotes ("). (Single quotes enclose characters, and are a different animal entirely.)

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

    //     7.4 String Initializers
    // We’ve already seen some examples with initializing string variables with string literals:

    char *s3 = "Hello, world!";
    char t[] = "Hello, again!";
    // But these two initializations are subtly different. A string literal, similar to an integer literal, has its memory automatically managed by the compiler for you! With an integer, i.e. a fixed size piece of data, the compiler can pretty easily manage it. But strings are a variable-byte beast which the compiler tames by tossing into a chunk of memory, and giving you a pointer to it.

    // char *s3 - This form points to wherever that string was placed. Typically, that place is in a land faraway from the rest of your program’s memory – read-only memory – for reasons related to performance & safety.

    // So, if you try to mutate that string with this:

    // s3[0] = 'z';  // BAD NEWS: tried to mutate a string literal!
    // The behavior is undefined. Probably, depending on your system, a crash will result.

    // But declaring it as an array is different. The compiler doesn’t stow those bytes in another part of town, they’re right down the street. This one is a mutable copy of the string – one we can change at will:

    // char t[] = "Hello, again!";  // t is an array copy of the string
    t[0] = 'z'; //  No problem

    printf("%s\n", t); // "zello, again!"
                       // So remember: if you have a pointer to a string literal, don’t try to change it! And if you use a string in double quotes to initialize an array, that’s not actually a string literal.

    // 7.5 Getting String Length
    //  There’s a function in <string.h> called strlen() that can be used to compute the length of any string in bytes65.
    // char is 1 byte in C because it is specified so in standards, ascii table, etc
    // - note that ascii is 7 bits/goes to 127, but 1 byte is standard/smallest memory allocation and is smallest that holds ascii

    char *s4 = "Hello, world!";

    printf("The string is %zu bytes long.\n", strlen(s4));

    // The strlen() function returns type size_t, which is an integer type so you can use it for integer math. We print size_t with %zu.

    // The above program prints:

    // The string is 13 bytes long.
    // Great! So it is possible to get the string length!

    // But… if C doesn’t track the length of the string anywhere, how does it know how long the string is?

    //     7.6 String Termination
    // C does strings a little differently than many programming languages, and in fact differently than almost every modern programming language.

    // When you’re making a new language, you have basically two options for storing a string in memory:

    // Store the bytes of the string along with a number indicating the length of the string.

    // Store the bytes of the string, and mark the end of the string with a special byte called the terminator.

    // If you want strings longer than 255 characters, option 1 requires at least two bytes to store the length. Whereas option 2 only requires one byte to terminate the string. So a bit of savings there.

    // Of course, these days it seems ridiculous to worry about saving a byte (or 3—lots of languages will happily let you have strings that are 4 gigabytes in length). But back in the day, it was a bigger deal.

    // So C took approach #2. In C, a “string” is defined by two basic characteristics:

    // A pointer to the first character in the string.
    // A zero-valued byte (or NUL character66) somewhere in memory after the pointer that indicates the end of the string.
    // A NUL character can be written in C code as \0, though you don’t often have to do this.

    // When you include a string in double quotes in your code, the NUL character is automatically, implicitly included.

    char *s5 = "Hello!"; // Actually "Hello!\0" behind the scenes
    // So with this in mind, let’s write our own strlen() function that counts chars in a string until it finds a NUL.
    // see outside of main:

    //     7.7 Copying a String
    //     If you want to make a copy of a string, you have to copy it a byte at a time—this means that you’re going to take the individual bytes of the string from one place in memory and duplicate them somewhere else in memory. This is made easier with the strcpy() function.68

    // Before you copy the string, make sure you have room to copy it into, i.e. the destination array that’s going to hold the characters needs to be at least as long as the string you’re copying.

    char s6[] = "Hello, world!";
    char t1[100]; // Each char is one byte, so plenty of room

    // This makes a copy of the string!
    strcpy(t1, s6);

    // We modify t
    t1[0] = 'z';

    // And s remains unaffected because it's a different string
    printf("%s\n", s6); // "Hello, world!"

    // But t has been changed
    printf("%s\n", t); // "zello, world!"

    // Notice with strcpy(), the destination pointer is the first argument, and the source pointer is the second. A mnemonic I use to remember this is that it’s the order you would have put t and s if an assignment = worked for strings, with the source on the right and the destination on the left.
}
// 7.6
// The procedure is to look down the string for a single NUL character, counting as we go67:

// note the single quotes for single char
int my_strlen(char *s) // note variable scope like in JS or other languages: scoped appropriately, so can be same name
{
    int count = 0;

    while (s[count] != '\0') // Single quotes for single char
        count++;

    return count;
}
// And that’s basically how the built-in strlen() gets the job done.
