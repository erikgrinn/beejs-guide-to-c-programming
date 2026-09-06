#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

*p = 12;  // Store something there

printf("%d\n", *p);  // Print it: 12

free(p);  // All done with that memory

//*p = 3490;  // ERROR: undefined behavior! Use after free()!
// Now, in that contrived example, there’s really no benefit to it. We could have just used an automatic int and it would have worked. But we’ll see how the ability to allocate memory this way has its advantages, especially with more complex data structures.

// One more thing you’ll commonly see takes advantage of the fact that sizeof can give you the size of the result type of any constant expression. So you could put a variable name in there, too, and use that. Here’s an example of that, just like the previous one:

int *p1 = malloc(sizeof *p1);  // *p is an int, so same as sizeof(int)
}