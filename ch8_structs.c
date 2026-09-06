#include <stdio.h>

// 8 Structs
// In C, we have something called a struct, which is a user-definable type that holds multiple pieces of data, potentially of different types.

// It’s a convenient way to bundle multiple variables into a single one. This can be beneficial for passing variables to functions (so you just have to pass one instead of many), and useful for organizing data and making code more readable.

// If you’ve come from another language, you might be familiar with the idea of classes and objects. These don’t exist in C, natively69. You can think of a struct as a class with only data members, and no methods.

// 8.1 Declaring a Struct
// You can declare a struct in your code like so:
struct car
{
    char *name;
    float price;
    int speed;
};

void set_price(struct car *c, float new_price);

// This is often done at the global scope outside any functions so that the struct is globally available.

// When you do this, you’re making a new type. The full type name is struct car. (Not just car—that won’t work.)

int main(void)
{

    // There aren’t any variables of that type yet, but we can declare some:

    struct car saturn; // Variable "saturn" of type "struct car"
    // And now we have an uninitialized variable saturn70 of type struct car.

    // We should initialize it! But how do we set the values of those individual fields?

    // Like in many other languages that stole it from C, we’re going to use the dot operator (.) to access the individual fields.
    saturn.name = "Saturn SL/2";
    saturn.price = 15999.99;
    saturn.speed = 175;

    printf("Name: %s\n", saturn.name);
    printf("Price (USD): %f\n", saturn.price);
    printf("Top Speed: %d km\n", saturn.speed);
    // There on the first lines, we set the values in the struct car, and then in the next bit, we print those values out.

    // 8.2 Struct Initializers
    //     That example in the previous section was a little unwieldy. There must be a better way to initialize that struct variable!

    // You can do it with an initializer by putting values in for the fields in the order they appear in the struct when you define the variable. (This won’t work after the variable has been defined—it has to happen in the definition).

    // Now with an initializer! Same field order as in the struct declaration:
    struct car saturn1 = {"Saturn SL/2", 16000.99, 175};

    printf("Name:      %s\n", saturn1.name);
    printf("Price:     %f\n", saturn1.price);
    printf("Top Speed: %d km\n", saturn1.speed);
    // The fact that the fields in the initializer need to be in the same order is a little freaky. If someone changes the order in struct car, it could break all the other code!

    // We can be more specific with our initializers:
    // faster way then one bye one from before:
    struct car saturn2 = {.speed = 175, .name = "Saturn SL/2"};
    printf("Name:      %s\n", saturn2.name);
    printf("Price:     %f\n", saturn2.price);
    printf("Top Speed: %d km\n", saturn2.speed);

    // 8.3 Passing Structs to Functions
    //     You can do a couple things to pass a struct to a function.

    // Pass the struct.
    // Pass a pointer to the struct.
    // Recall that when you pass something to a function, a copy of that thing gets made for the function to operate on, whether it’s a copy of a pointer, an int, a struct, or anything.

    // There are basically two cases when you’d want to pass a pointer to the struct:

    // You need the function to be able to make changes to the struct that was passed in, and have those changes show in the caller.
    // The struct is somewhat large and it’s more expensive to copy that onto the stack than it is to just copy a pointer71.
    // For those two reasons, it’s far more common to pass a pointer to a struct to a function, though its by no means illegal to pass the struct itself.

    // Let’s try passing in a pointer, making a function that will allow you to set the .price field of the struct car:
    struct car saturn3 = {.speed = 175, .name = "Saturn SL/2"};

    // Pass a pointer to this struct car, along with a new,
    // more realistic, price:
    set_price(&saturn3, 799.99);

    printf("Price: %f\n", saturn3.price);
    //     You should be able to come up with the function signature for set_price() just by looking at the types of the arguments we have there.

    // saturn is a struct car, so &saturn must be the address of the struct car, AKA a pointer to a struct car, namely a struct car*.

    // And 799.99 is a float.

    // So the function declaration must look like this:
    // void set_price(struct car *c, float new_price);
    // see out of main (as well as for 8.4):

    // 8.5 Copying and Returning structs
    // Here’s an easy one for you!

    // Just assign from one to the other!

    struct car a, b;

    b = a; // Copy the struct
    // And returning a struct (as opposed to a pointer to one) from a function also makes a similar copy to the receiving variable.

    // This is not a “deep copy”72. All fields are copied as-is, including pointers to things.

    // 8.6 Comparing structs
    // There’s only one safe way to do it: compare each field one at a time.

    // You might think you could use memcmp()73, but that doesn’t handle the case of the possible padding bytes that might be in there.

    // If you clear the struct to zero first with memset()74, then it might work, though there could be weird elements that might not compare as you expect75.
}

// 8.3
//  We just need to write the body. One attempt might be:

// void set_price(struct car *c, float new_price)
// {
//     c.price = new_price; // ERROR!!: c shows the error - expression must have struct or union type but it has type "struct car *"
// }

// That won’t work because the dot operator only works on structs… it doesn’t work on pointers to structs.

// Ok, so we can dereference the variable c to de-pointer it to get to the struct itself. Dereferencing a struct car* results in the struct car that the pointer points to, which we should be able to use the dot operator on:

void set_price(struct car *c, float new_price)
{
    (*c).price = new_price; // Works, but is ugly and non-idiomatic :(
}
// And that works! But it’s a little clunky to type all those parens and the asterisk. C has some syntactic sugar called the arrow operator that helps with that.

// 8.4 Arrow Operator
//  The arrow operator helps refer to fields in pointers to structs.

void set_price1(struct car *c, float new_price)
{
    // (*c).price = new_price;  // Works, but non-idiomatic :(
    //
    // The line above is 100% equivalent to the one below:

    c->price = new_price; // That's the one!
}
// So when accessing fields, when do we use dot and when do we use arrow?

// If you have a struct, use dot (.).
// If you have a pointer to a struct, use arrow (->).
