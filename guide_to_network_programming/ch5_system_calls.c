#define _GNU_SOURCE // to fix codespace linting/intellisense
// can also use below for more direct target:
//  #define _POSIX_C_SOURCE 200112L  // Unlocks getaddrinfo and modern sockets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void)
{
    // 5.1 getaddrinfo()
    // This is a real workhorse of a function with a lot of options, but usage is actually pretty simple. It helps set up the structs you need later on.

    // A tiny bit of history: it used to be that you would use a function called gethostbyname() to do DNS lookups. Then you’d load that information by hand into a struct sockaddr_in, and use that in your calls.

    // This is no longer necessary, thankfully. (Nor is it desirable, if you want to write code that works for both IPv4 and IPv6!) In these modern times, you now have the function getaddrinfo() that does all kinds of good stuff for you, including DNS and service name lookups, and fills out the structs you need, besides!

    // int getaddrinfo(const char *node,    // e.g. "www.example.com" or IP
    //                 const char *service, // e.g. "http" or port number
    //                 const struct addrinfo *hints,
    //                 struct addrinfo **res);

    // You give this function three input parameters, and it gives you a pointer to a linked-list, res, of results.

    // Here’s a sample call if you’re a server who wants to listen on your host’s IP address, port 3490. Note that this doesn’t actually do any listening or network setup; it merely sets up structures we’ll use later:
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo; // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    // servinfo now points to a linked list of 1 or more
    // struct addrinfos

    // ... do everything until you don't need servinfo anymore ....

    // Finally, when we’re eventually all done with the linked list that getaddrinfo() so graciously allocated for us, we can (and should) free it all up with a call to freeaddrinfo().
    freeaddrinfo(servinfo); // free the linked-list

    // Here’s a sample call if you’re a client who wants to connect to a particular server, say “www.example.net” port 3490. Again, this doesn’t actually connect, but it sets up the structures we’ll use later:

    // int status;
    // struct addrinfo hints;
    // struct addrinfo *servinfo;  // will point to the results

    // memset(&hints, 0, sizeof hints); // make sure the struct is empty
    // hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    // hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    // get ready to connect
    status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);

    // servinfo now points to a linked list of 1 or more
    // struct addrinfos

    // etc.
    freeaddrinfo(servinfo); // free the linked-list
}
