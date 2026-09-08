#include <stdio.h>

// We’ve already seen some examples of I/O with printf() for doing I/O at the console.

// But we’ll push those concepts a little farther this chapter.

// 9.1 The FILE* Data Type
// When we do any kind of I/O in C, we do so through a piece of data that you get in the form of a FILE* type. This FILE* holds all the information needed to communicate with the I/O subsystem about which file you have open, where you are in the file, and so on.

// The spec refers to these as streams, i.e. a stream of data from a file or from any source. I’m going to use “files” and “streams” interchangeably, but really you should think of a “file” as a special case of a “stream”. There are other ways to stream data into a program than just reading from a file.

// We’ll see in a moment how to go from having a filename to getting an open FILE* for it, but first I want to mention three streams that are already open for you and ready for use.

// FILE* name	Description
// stdin	Standard Input, generally the keyboard by default
// stdout	Standard Output, generally the screen by default
// stderr	Standard Error, generally the screen by default, as well
// We’ve actually been using these implicitly already, it turns out. For example, these two calls are the same:

// printf("Hello, world!\n");
// fprintf(stdout, "Hello, world!\n");  // printf to a file
// But more on that later.

// Also you’ll notice that both stdout and stderr go to the screen. While this seems at first either like an oversight or redundancy, it actually isn’t. Typical operating systems allow you to redirect the output of either of those into different files, and it can be convenient to be able to separate error messages from regular non-error output.

// For example, in a POSIX shell (like sh, ksh, bash, zsh, etc.) on a Unix-like system, we could run a program and send just the non-error (stdout) output to one file, and all the error (stderr) output to another file.

// ./foo > output.txt 2> errors.txt   # This command is Unix-specific
// For this reason, you should send serious error messages to stderr instead of stdout.

// More on how to do that later.

int main(void)
{
    //     9.2 Reading Text Files
    // Streams are largely categorized two different ways: text and binary.

    // Text streams are allowed to do significant translation of the data, most notably translations of newlines to their different representations76. Text files are logically a sequence of lines separated by newlines. To be portable, your input data should always end with a newline.

    // But the general rule is that if you’re able to edit the file in a regular text editor, it’s a text file. Otherwise, it’s binary. More on binary later.

    // So let’s get to work—how do we open a file for reading, and pull data out of it?

    // Let’s create a file called hello.txt that has just this in it:

    // Hello, world!

    // FILE *fp; // Variable to represent open file

    // fp = fopen("hello.txt", "r"); // Open file for reading

    // int c = fgetc(fp); // Read a single character
    // printf("%c\n", c); // Print char to stdout

    // fclose(fp); // Close the file when done

    // See how when we opened the file with fopen(), it returned the FILE* to us so we could use it later.

    // (I’m leaving it out for brevity, but fopen() will return NULL if something goes wrong, like file-not-found, so you should really error check it!)

    // Also notice the "r" that we passed in—this means “open a text stream for reading”. (There are various strings we can pass to fopen() with additional meaning, like writing, or appending, and so on.)

    // After that, we used the fgetc() function to get a character from the stream. You might be wondering why I’ve made c an int instead of a char—hold that thought!

    // Finally, we close the stream when we’re done with it. All streams are automatically closed when the program exits, but it’s good form and good housekeeping to explicitly close any files yourself when done with them.

    // The FILE* keeps track of our position in the file. So subsequent calls to fgetc() would get the next character in the file, and then the next, until the end.

    // But that sounds like a pain. Let’s see if we can make it easier.

    //     9.3 End of File: EOF
    // There is a special character defined as a macro: EOF. This is what fgetc() will return when the end of the file has been reached and you’ve attempted to read another character.

    // How about I share that Fun Fact™, now. Turns out EOF is the reason why fgetc() and functions like it return an int instead of a char.
    // EOF isn’t a character proper, and its value likely falls outside the range of char.
    //  * Since fgetc() needs to be able to return any byte and EOF, it needs to be a wider type that can hold more values. so int it is.
    //  But unless you’re comparing the returned value against EOF, you can know, deep down, it’s a char.

    // All right! Back to reality! We can use this to read the whole file in a loop.

    FILE *fp;
    int c;

    fp = fopen("hello.txt", "r");

    while ((c = fgetc(fp)) != EOF)
        printf("%c", c);

    fclose(fp);

    // But still, we’re operating a character at a time, and lots of text files make more sense at the line level. Let’s switch to that.

    //     9.3.1 Reading a Line at a Time
    // So how can we get an entire line at once? fgets() to the rescue! For arguments, it takes a pointer to a char buffer to hold bytes, a maximum number of bytes to read, and a FILE* to read from. It returns NULL on end-of-file or error. fgets() is even nice enough to NUL-terminate the string when its done77.

    // Let’s do a similar loop as before, except let’s have a multiline file and read it in a line at a time.

    // Here’s a file quote.txt:

    // A wise man can learn more from
    // a foolish question than a fool
    // can learn from a wise answer.
    //                   --Bruce Lee

    FILE *fp1;
    char s1[1024]; // Big enough for any line this program will encounter
    int linecount = 0;

    fp1 = fopen("quote.txt", "r");

    while (fgets(s1, sizeof s1, fp1) != NULL)
        printf("%d: %s", ++linecount, s1);

    fclose(fp1);
}
