#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for memset and memcpy

// This is one of the big areas where C likely diverges from languages you already know: manual memory management.

// Other languages uses reference counting, garbage collection, or other means to determine when to allocate new memory for some data—and when to deallocate it when no variables refer to it.

// And that’s nice. It’s nice to be able to not worry about it, to just drop all the references to an item and trust that at some point the memory associated with it will be freed.

// But C’s not like that, entirely.

// Of course, in C, some variables are automatically allocated and deallocated when they come into scope and leave scope. We call these automatic variables. They’re your average run-of-the-mill block scope “local” variables. No problem.

// But what if you want something to persist longer than a particular block? This is where manual memory management comes into play.

// You can tell C explicitly to allocate for you a certain number of bytes that you can use as you please. And these bytes will remain allocated until you explicitly free that memory90.

// It’s important to free the memory you’re done with! If you don’t, we call that a memory leak and your process will continue to reserve that memory until it exits.

// *If you manually allocated it, you have to manually free it when you’re done with it.*

// So how do we do this? We’re going to learn a couple new functions, and make use of the sizeof operator to help us learn how many bytes to allocate.

// In common C parlance, devs say that automatic local variables are allocated “on the stack”, and manually-allocated memory is “on the heap”. The spec doesn’t talk about either of those things, but all C devs will know what you’re talking about if you bring them up.

// All functions we’re going to learn in this chapter can be found in <stdlib.h>.

int main(void)
{
    //     12.1 Allocating and Deallocating, malloc() and free()
    // The malloc() function accepts a number of bytes to allocate, and returns a void pointer to that block of newly-allocated memory.

    // Since it’s a void*, you can assign it into whatever pointer type you want… normally this will correspond in some way to the number of bytes you’re allocating.

    // So… how many bytes should I allocate? We can use sizeof to help with that. If we want to allocate enough room for a single int, we can use sizeof(int) and pass that to malloc().

    // After we’re done with some allocated memory, we can call free() to indicate we’re done with that memory and it can be used for something else. As an argument, you pass the same pointer you got from malloc() (or a copy of it). It’s undefined behavior to use a memory region after you free() it.

    // Let’s try. We’ll allocate enough memory for an int, and then store something there, and then print it.

    // Allocate space for a single int (sizeof(int) bytes-worth):

    int *p = malloc(sizeof(int));

    *p = 12; // Store something there

    printf("%d\n", *p); // Print it: 12

    free(p); // All done with that memory

    //*p = 3490;  // ERROR: undefined behavior! Use after free()!
    // Now, in that contrived example, there’s really no benefit to it. We could have just used an automatic int and it would have worked. But we’ll see how the ability to allocate memory this way has its advantages, especially with more complex data structures.

    // One more thing you’ll commonly see takes advantage of the fact that sizeof can give you the size of the result type of any constant expression. So you could put a variable name in there, too, and use that. Here’s an example of that, just like the previous one:

    int *p1 = malloc(sizeof *p1); // *p is an int, so same as sizeof(int)

    // 12.2 Error Checking
    // All the allocation functions return a pointer to the newly-allocated stretch of memory, or NULL if the memory cannot be allocated for some reason.

    // Some OSes like Linux can be configured in such a way that malloc() never returns NULL, even if you’re out of memory. But despite this, you should always code it up with protections in mind.

    int *x;

    x = malloc(sizeof(int) * 10);

    if (x == NULL)
    {
        printf("Error allocating 10 ints\n");
        // do something here to handle it
    }
    // Here’s a common pattern that you’ll see, where we do the assignment and the condition on the same line:

    int *x1;

    if ((x1 = malloc(sizeof(int) * 10)) == NULL)
    {
        printf("Error allocating 10 ints\n");
        // do something here to handle it
    }

    // 12.3 Allocating Space for an Array
    // We’ve seen how to allocate space for a single thing; now what about for a bunch of them in an array?

    // In C, an array is a bunch of the same thing back-to-back in a contiguous stretch of memory.

    // We can allocate a contiguous stretch of memory—we’ve seen how to do that. If we wanted 3490 bytes of memory, we could just ask for it:

    char *p2 = malloc(3490); // Voila

    // And—indeed!—that’s an array of 3490 chars (AKA a string!) since each char is 1 byte. In other words, sizeof(char) is 1.

    // Note: there’s no initialization done on the newly-allocated memory—it’s full of garbage. Clear it with memset() if you want to, or see calloc(), below.

    // But we can just multiply the size of the thing we want by the number of elements we want, and then access them using either pointer or array notation. Example!

    // Allocate space for 10 ints
    int *p3 = malloc(sizeof(int) * 10);

    // Assign them values 0-45:
    for (int i = 0; i < 10; i++)
        p3[i] = i * 5;

    // Print all values 0, 5, 10, 15, ..., 40, 45
    for (int i = 0; i < 10; i++)
        printf("%d\n", p3[i]);

    // Free the space
    free(p3);

    // The key’s in that malloc() line. If we know each int takes sizeof(int) bytes to hold it, and we know we want 10 of them, we can just allocate exactly that many bytes with sizeof(int) * 10

    // And this trick works for every type. Just pass it to sizeof and multiply by the size of the array.

    // 12.4 An Alternative: calloc()
    // This is another allocation function that works similarly to malloc(), with two key differences:

    // Instead of a single argument, you pass the size of one element, and the number of elements you wish to allocate. It’s like it’s made for allocating arrays.
    // It clears the memory to zero.
    // You still use free() to deallocate memory obtained through calloc().

    // Here’s a comparison of calloc() and malloc().

    // Allocate space for 10 ints with calloc(), initialized to 0:
    int *p4 = calloc(10, sizeof(int));

    // Allocate space for 10 ints with malloc(), initialized to 0:
    int *q = malloc(10 * sizeof(int));
    memset(q, 0, 10 * sizeof(int)); // set to 0
    // Again, the result is the same for both except malloc() doesn’t zero the memory by default.

    //     12.5 Changing Allocated Size with realloc()
    // If you’ve already allocated 10 ints, but later you decide you need 20, what can you do?

    // One option is to allocate some new space, and then memcpy() the memory over… but it turns out that sometimes you don’t need to move anything. And there’s one function that’s just smart enough to do the right thing in all the right circumstances: realloc().

    // It takes a pointer to some previously-allocted memory (by malloc() or calloc()) and a new size for the memory region to be.

    // It then grows or shrinks that memory, and returns a pointer to it. Sometimes it might return the same pointer (if the data didn’t have to be copied elsewhere), or it might return a different one (if the data did have to be copied).

    // Be sure when you call realloc(), you specify the number of bytes to allocate, and not just the number of array elements! That is:

    // num_floats *= 2;

    // np = realloc(p, num_floats);  // WRONG: need bytes, not number of elements!

    // np = realloc(p, num_floats * sizeof(float));  // Better!

    // Let’s allocate an array of 20 floats, and then change our mind and make it an array of 40.

    // We’re going to assign the return value of realloc() into another pointer so we can check if it’s NULL. If it’s not, then we can reassign it into our original pointer. (If we just assigned the return value directly into the original pointer, we’d lose that pointer if the function returned NULL and we’d have no way to get it back.)

    // Allocate space for 20 floats
    float *p5 = malloc(sizeof *p5 * 20); // sizeof *p same as sizeof(float)

    // Assign them fractional values 0.0-1.0:
    for (int i = 0; i < 20; i++)
        p5[i] = i / 20.0;

    // But wait! Let's actually make this an array of 40 elements
    float *new_p = realloc(p5, sizeof *p5 * 40);

    // Check to see if we successfully reallocated
    if (new_p == NULL)
    {
        printf("Error reallocing\n");
        return 1;
    }

    // If we did, we can just reassign p
    p5 = new_p;

    // And assign the new elements values in the range 1.0-2.0
    for (int i = 20; i < 40; i++)
        p5[i] = 1.0 + (i - 20) / 20.0;

    // Print all values 0.0-2.0 in the 40 elements:
    for (int i = 0; i < 40; i++)
        printf("%f\n", p5[i]);

    // Free the space
    free(p5);

    //     Notice in there how we took the return value from realloc() and reassigned it into the same pointer variable p that we passed in. That’s pretty common to do.

    // Also if line 7 is looking weird, with that sizeof *p in there, remember that sizeof works on the size of the type of the expression. And the type of *p is float, so that line is equivalent to sizeof(float).

    // Finally, it might be a little weird that I don’t have a free(new_p) in there anywhere, even though that was the pointer returned by realloc(). The reason is that we copy new_p into p on line 23, so they both have the same value; that is, they both point to the same chunk of memory, and there’s only the one chunk. So when I free(), I could actually free either of them for the same effect.
}

// 12.5.1 Reading in Lines of Arbitrary Length
// I want to demonstrate two things with this full-blown example.
// (this uses file i/o chapter topics)

// Use of realloc() to grow a buffer as we read in more data.
// Use of realloc() to shrink the buffer down to the perfect size after we’ve completed the read.
// What we see here is a loop that calls fgetc() over and over to append to a buffer until we see that the last character is a newline.

// Once it finds the newline, it shrinks the buffer to just the right size and returns it.

// #include <stdio.h>
// #include <stdlib.h>

// // Read a line of arbitrary size from a file
// //
// // Returns a pointer to the line.
// // Returns NULL on EOF or error.
// //
// // It's up to the caller to free() this pointer when done with it.
// //
// // Note that this strips the newline from the result. If you need
// // it in there, probably best to switch this to a do-while.

// char *readline(FILE *fp)
// {
//     int offset = 0;   // Index next char goes in the buffer
//     int bufsize = 4;  // Preferably power of 2 initial size
//     char *buf;        // The buffer
//     int c;            // The character we've read in

//     buf = malloc(bufsize);  // Allocate initial buffer

//     if (buf == NULL)   // Error check
//         return NULL;

//     // Main loop--read until newline or EOF
//     while (c = fgetc(fp), c != '\n' && c != EOF) {

//         // Check if we're out of room in the buffer accounting
//         // for the extra byte for the NUL terminator
//         if (offset == bufsize - 1) {  // -1 for the NUL terminator
//             bufsize *= 2;  // 2x the space

//             char *new_buf = realloc(buf, bufsize);

//             if (new_buf == NULL) {
//                 free(buf);   // On error, free and bail
//                 return NULL;
//             }

//             buf = new_buf;  // Successful realloc
//         }

//         buf[offset++] = c;  // Add the byte onto the buffer
//     }

//     // We hit newline or EOF...

//     // If at EOF and we read no bytes, free the buffer and
//     // return NULL to indicate we're at EOF:
//     if (c == EOF && offset == 0) {
//         free(buf);
//         return NULL;
//     }

//     // Shrink to fit
//     if (offset < bufsize - 1) {  // If we're short of the end
//         char *new_buf = realloc(buf, offset + 1); // +1 for NUL terminator

//         // If successful, point buf to new_buf;
//         // otherwise we'll just leave buf where it is
//         if (new_buf != NULL)
//             buf = new_buf;
//     }

//     // Add the NUL terminator
//     buf[offset] = '\0';

//     return buf;
// }

// int main(void)
// {
//     FILE *fp = fopen("foo.txt", "r");

//     char *line;

//     while ((line = readline(fp)) != NULL) {
//         printf("%s\n", line);
//         free(line);
//     }

//     fclose(fp);
// }
// When growing memory like this, it’s common (though hardly a law) to double the space needed each step just to minimize the number of realloc()s that occur.

// Finally you might note that readline() returns a pointer to a malloc()d buffer. As such, it’s up to the caller to explicitly free() that memory when it’s done with it.

// 12.5.2 realloc() with NULL
// Trivia time! These two lines are equivalent:

// char *p = malloc(3490);
// char *p = realloc(NULL, 3490);
// That could be convenient if you have some kind of allocation loop and you don’t want to special-case the first malloc().

// int *p = NULL;
// int length = 0;

// while (!done) {
//     // Allocate 10 more ints:
//     length += 10;
//     p = realloc(p, sizeof *p * length);

//     // Do amazing things
//     // ...
// }
// In that example, we didn’t need an initial malloc() since p was NULL to start.

// 12.6 Aligned Allocations
// You probably aren’t going to need to use this.