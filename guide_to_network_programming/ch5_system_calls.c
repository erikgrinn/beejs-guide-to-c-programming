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
    // 5.1 getaddrinfo() - Prepare to Launch
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

    int status1;
    struct addrinfo hints1;
    struct addrinfo *servinfo1; // will point to the results

    memset(&hints1, 0, sizeof hints1); // make sure the struct is empty
    hints1.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
    hints1.ai_socktype = SOCK_STREAM;  // TCP stream sockets

    // get ready to connect
    status1 = getaddrinfo("www.example.net", "3490", &hints1, &servinfo1);

    // servinfo now points to a linked list of 1 or more
    // struct addrinfos

    // etc.
    freeaddrinfo(servinfo1); // free the linked-list

    // 5.2 socket() - Get the File Descriptor
    int socket(int domain, int type, int protocol);
    // But what are these arguments? They allow you to say what kind of socket you want (IPv4 or IPv6, stream or datagram, and TCP or UDP).

    // What you really want to do is use the values from the results of the call to getaddrinfo(), and feed them into socket() directly like this:
    int s;
    struct addrinfo hints2, *res2;
    memset(&hints2, 0, sizeof hints2);
    hints2.ai_family = AF_UNSPEC;
    hints2.ai_socktype = SOCK_STREAM;

    // do the lookup
    // [pretend we already filled out the "hints" struct]
    getaddrinfo("www.example.com", "http", &hints2, &res2);

    // again, you should do error-checking on getaddrinfo(), and walk
    // the "res" linked list looking for valid entries instead of just
    // assuming the first one is good (like many of these examples do).
    // See the section on client/server for real examples.

    s = socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);

    freeaddrinfo(res2);

    // socket() simply returns to you a socket descriptor that you can use in later system calls, or -1 on error. The global variable errno is set to the error’s value (see the errno man page for more details, and a quick note on using errno in multithreaded programs).
    // Fine, fine, fine, but what good is this socket? The answer is that it’s really no good by itself, and you need to read on and make more system calls for it to make any sense.

    // 5.3 bind() — What port am I on?
    // Once you have a socket, you might have to associate that socket with a port on your local machine. (This is commonly done if you’re going to listen() for incoming connections on a specific port—multiplayer network games do this when they tell you to “connect to 192.168.5.10 port 3490”.) The port number is used by the kernel to match an incoming packet to a certain process’s socket descriptor. If you’re going to only be doing a connect() (because you’re the client, not the server), this is probably unnecessary. Read it anyway, just for kicks.
    // int bind(int sockfd, struct sockaddr *my_addr, int addrlen); - this is outdated and shows warning/error
    int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);

    // sockfd is the socket file descriptor returned by socket(). my_addr is a pointer to a struct sockaddr that contains information about your address, namely, port and IP address. addrlen is the length in bytes of that address.

    // Whew. That’s a bit to absorb in one chunk. Let’s have an example that binds the socket to the host the program is running on, port 3490:
    struct addrinfo hints3, *res3;
    int sockfd;

    // first, load up address structs with getaddrinfo():

    memset(&hints3, 0, sizeof hints3);
    hints3.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints3.ai_socktype = SOCK_STREAM;
    hints3.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo(NULL, "3490", &hints3, &res3);

    // make a socket:

    sockfd = socket(res3->ai_family, res3->ai_socktype, res3->ai_protocol);

    // bind it to the port we passed in to getaddrinfo():

    bind(sockfd, res3->ai_addr, res3->ai_addrlen);
}
