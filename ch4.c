
#include <stdio.h>

// define/declare before main
int plus_one(int n) // The "definition". argument is *copied* into paramter n
{
    return n + 1;
}

void hello(void)
{
    printf("Hello, world!\n");
}

// 4.1 passing by value/copy
void increment(int a)
{
    a++;
}

int main(void)
{
    int i = 10, j;

    j = plus_one(i); // The "call"

    printf("i + 1 is %d\n", j);

    hello();

    // 4.1
    increment(i);
    printf("i==%d\n", i); //prints i==10 still
}

//4.2/4.3 function prototype/void (empty parameter list)
// basically can define function quickly at top before main but then flesh it out 
// later one so compiler doesn't get upset
// one caveat/important detail
// void foo();
// void foo(void);  // Not the same! especially for function prototypes


// #include <stdio.h>

// int foo(void);  // This is the prototype!

// int main(void)
// {
//     int i;
    
//     // We can call foo() here before it's definition because the
//     // prototype has already been declared, above!

//     i = foo();
    
//     printf("%d\n", i);  // 3490
// }

// int foo(void)  // This is the definition, just like the prototype!
// {
//     return 3490;
// }