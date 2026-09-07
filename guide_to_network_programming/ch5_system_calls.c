#define _GNU_SOURCE // to fix codespace linting/intellisense
// can also use below for more direct target:
//  #define _POSIX_C_SOURCE 200112L  // Unlocks getaddrinfo and modern sockets

// for accept/5.6:
#define MYPORT "3490" // the port users will be connecting to
#define BACKLOG 10    // how many pending connections queue holds

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
    // One small extra final note about bind(): there are times when you won’t absolutely have to call it. If you are connect()ing to a remote machine and you don’t care what your local port is (as is the case with telnet where you only care about the remote port), you can simply call connect(), it’ll check to see if the socket is unbound, and will bind() it to an unused local port if necessary.

    // 5.4 connect() - Hey you
    // Let’s just pretend for a few minutes that you’re a telnet application. Your user commands you (just like in the movie TRON) to get a socket file descriptor. You comply and call socket(). Next, the user tells you to connect to “10.12.110.57” on port “23” (the standard telnet port). Yow! What do you do now?

    // Lucky for you, program, you’re now perusing the section on connect()—how to connect to a remote host. So read furiously onward! No time to lose!

    // The connect() call is as follows:

    // int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);  -used but outdated
    int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen); // sockfd is our friendly neighborhood socket file descriptor, as returned by the socket() call, serv_addr is a struct sockaddr containing the destination port and IP address, and addrlen is the length in bytes of the server address structure.

    // All of this information can be gleaned from the results of the getaddrinfo() call, which rocks.

    // Is this starting to make more sense
    // ? I can’t hear you from here,
    // so I’ll just have to hope that it is.Let’s have an example where we make a socket connection to “www.example.com”, port 3490 :

    struct addrinfo hints4, *res4;
    int sockfd1;

    // first, load up address structs with getaddrinfo():

    memset(&hints4, 0, sizeof hints4);
    hints4.ai_family = AF_UNSPEC;
    hints4.ai_socktype = SOCK_STREAM;

    getaddrinfo("www.example.com", "3490", &hints4, &res4);

    // make a socket:

    sockfd1 = socket(res4->ai_family, res4->ai_socktype, res4->ai_protocol);

    // connect!

    connect(sockfd1, res4->ai_addr, res4->ai_addrlen);
    //     Again, old - school programs filled out their own struct sockaddr_ins to pass to connect().You can do that if you want to.See the similar note in the bind() section, above.

    //    Be sure to check the return value from connect()—it’ll return -1 on error and set the variable errno.

    //     Also, notice that we didn’t call bind().Basically, we don’t care about our local port number; we only care where we’re going (the remote port). The kernel will choose a local port for us, and the site we connect to will automatically get this information from us. No worries.

    // 5.5 listen() — Will somebody please call me?
    // OK, time for a change of pace. What if you don’t want to connect to a remote host. Say, just for kicks, that you want to wait for incoming connections and handle them in some way. The process is two step: first you listen(), then you accept() (see below).

    // The listen() call is fairly simple, but requires a bit of explanation:

    int listen(int sockfd, int backlog);
    // sockfd is the usual socket file descriptor from the socket() system call. backlog is the number of connections allowed on the incoming queue. What does that mean? Well, incoming connections are going to wait in this queue until you accept() them (see below) and this is the limit on how many can queue up. Most systems silently limit this number to about 20; you can probably get away with setting it to 5 or 10.

    // Again, as per usual, listen() returns -1 and sets errno on error.

    // Well, as you can probably imagine, we need to call bind() before we call listen() so that the server is running on a specific port. (You have to be able to tell your buddies which port to connect to!) So if you’re going to be listening for incoming connections, the sequence of system calls you’ll make is:

    // getaddrinfo();
    // socket();
    // bind();
    // listen();
    /* accept() goes here */
    // I’ll just leave that in the place of sample code, since it’s fairly self-explanatory. (The code in the accept() section, below, is more complete.) The really tricky part of this whole sha-bang is the call to accept().

    // 5.6 accept()—“Thank you for calling port 3490.”
    // Get ready—the accept() call is kinda weird! What’s going to happen is this: someone far far away will try to connect() to your machine on a port that you are listen()ing on. Their connection will be queued up waiting to be accept()ed. You call accept() and you tell it to get the pending connection. It’ll return to you a brand new socket file descriptor to use for this single connection! That’s right, suddenly you have two socket file descriptors for the price of one! The original one is still listening for more new connections, and the newly created one is finally ready to send() and recv(). We’re there!

    // The call is as follows:

    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    // sockfd is the listen()ing socket descriptor. Easy enough. addr will usually be a pointer to a local struct sockaddr_storage. This is where the information about the incoming connection will go (and with it you can determine which host is calling you from which port). addrlen is a local integer variable that should be set to sizeof(struct sockaddr_storage) before its address is passed to accept(). accept() will not put more than that many bytes into addr. If it puts fewer in, it’ll change the value of addrlen to reflect that.

    // Guess what? accept() returns -1 and sets errno if an error occurs. Betcha didn’t figure that.

    // Like before, this is a bunch to absorb in one chunk, so here’s a sample code fragment for your perusal:

    // #define MYPORT "3490"  // the port users will be connecting to
    // #define BACKLOG 10     // how many pending connections queue holds

    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints5, *res5;
    int sockfd2, new_fd;

    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints5, 0, sizeof hints5);
    hints5.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints5.ai_socktype = SOCK_STREAM;
    hints5.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo(NULL, MYPORT, &hints5, &res5);

    // make a socket, bind it, and listen on it:

    sockfd2 = socket(res5->ai_family, res5->ai_socktype,
                     res5->ai_protocol);
    bind(sockfd2, res5->ai_addr, res5->ai_addrlen);
    listen(sockfd2, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd2, (struct sockaddr *)&their_addr,
                    &addr_size);

    // ready to communicate on socket descriptor new_fd!
    // .
    // .
    // .
    // Again, note that we will use the socket descriptor new_fd for all send() and recv() calls. If you’re only getting one single connection ever, you can close() the listening sockfd in order to prevent more incoming connections on the same port, if you so desire.

    //  5.7 send() and recv()—Talk to me, baby!
    // These two functions are for communicating over stream sockets or connected datagram sockets. If you want to use regular unconnected datagram sockets, you’ll need to see the section on sendto() and recvfrom(), below.

    // Here’s something that might (or might not) be new to you: these are blocking calls. That is, recv() will block until there is some data ready to receive. “But what does ‘block’ mean, already?!” It means your program is going to stop there, on that system call, until someone sends you something. (The OS techie jargon for “stop” in that sentence is actually sleep, so I might use those terms interchangeably.) send() can also block if the stuff you’re sending is all jammed up somehow, but that’s rarer. We’ll revisit this concept later, and talk about how to avoid it when you need to.

    // Here’s the send() call:

    // int send(int sockfd, const void *msg, int len, int flags); used but outdated
    ssize_t send(int sockfd, const void *buf, size_t len, int flags);

    // sockfd is the socket descriptor you want to send data to (whether it’s the one returned by socket() or the one you got with accept()). msg is a pointer to the data you want to send, and len is the length of that data in bytes. Just set flags to 0. (See the send() man page for more information concerning flags.)

    // Some sample code might be:

    char *msg = "Beej was here!";
    int len, bytes_sent;
    // .
    // .
    // .
    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);
    // .
    // .
    // .
    // send() returns the number of bytes actually sent out—this might be less than the number you told it to send! See, sometimes you tell it to send a whole gob of data and it just can’t handle it. It’ll fire off as much of the data as it can, and trust you to send the rest later. Remember, if the value returned by send() doesn’t match the value in len, it’s up to you to send the rest of the string. The good news is this: if the packet is small (less than 1K or so) it will probably manage to send the whole thing all in one go. Again, -1 is returned on error, and errno is set to the error number.

    // The recv() call is similar in many respects:

    // int recv(int sockfd, void *buf, int len, int flags); used but outdated
    ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    // sockfd is the socket descriptor to read from, buf is the buffer to read the information into, len is the maximum length of the buffer, and flags can again be set to 0. (See the recv() man page for flag information.)

    // recv() returns the number of bytes actually read into the buffer, or -1 on error (with errno set, accordingly).

    // Wait! recv() can return 0. This can mean only one thing: the remote side has closed the connection on you! A return value of 0 is recv()’s way of letting you know this has occurred.

    // There, that was easy, wasn’t it? You can now pass data back and forth on stream sockets! Whee! You’re a Unix Network Programmer!

    //     5.8 sendto() and recvfrom()—Talk to me, DGRAM-style
    // “This is all fine and dandy,” I hear you saying, “but where does this leave me with unconnected datagram sockets?” No problemo, amigo. We have just the thing.

    // Since datagram sockets aren’t connected to a remote host, guess which piece of information we need to give before we send a packet? That’s right! The destination address! Here’s the scoop:

    // int sendto(int sockfd, const void *msg, int len, unsigned int flags,
    //            const struct sockaddr *to, socklen_t tolen); used but outdated
    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                   const struct sockaddr *dest_addr, socklen_t addrlen);

    // As you can see, this call is basically the same as the call to send() with the addition of two other pieces of information. to is a pointer to a struct sockaddr (which will probably be another struct sockaddr_in or struct sockaddr_in6 or struct sockaddr_storage that you cast at the last minute) which contains the destination IP address and port. tolen, an int deep-down, can simply be set to sizeof *to or sizeof(struct sockaddr_storage).

    // To get your hands on the destination address structure, you’ll probably either get it from getaddrinfo(), or from recvfrom(), below, or you’ll fill it out by hand.

    // Just like with send(), sendto() returns the number of bytes actually sent (which, again, might be less than the number of bytes you told it to send!), or -1 on error.

    // Equally similar are recv() and recvfrom(). The synopsis of recvfrom() is:

    // int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
    //              struct sockaddr *from, int *fromlen); used but outdated
    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                     struct sockaddr *src_addr, socklen_t *addrlen);

    // Again, this is just like recv() with the addition of a couple fields. from is a pointer to a local struct sockaddr_storage that will be filled with the IP address and port of the originating machine. fromlen is a pointer to a local int that should be initialized to sizeof *from or sizeof(struct sockaddr_storage). When the function returns, fromlen will contain the length of the address actually stored in from.

    // recvfrom() returns the number of bytes received, or -1 on error (with errno set accordingly).

    // So, here’s a question: why do we use struct sockaddr_storage as the socket type? Why not struct sockaddr_in? Because, you see, we want to not tie ourselves down to IPv4 or IPv6. So we use the generic struct sockaddr_storage which we know will be big enough for either.

    // (So… here’s another question: why isn’t struct sockaddr itself big enough for any address? We even cast the general-purpose struct sockaddr_storage to the general-purpose struct sockaddr! Seems extraneous and redundant, huh? The answer is, it just isn’t big enough, and I’d guess that changing it at this point would be Problematic. So they made a new one.)

    // Remember, if you connect() a datagram socket, you can then simply use send() and recv() for all your transactions. The socket itself is still a datagram socket and the packets still use UDP, but the socket interface will automatically add the destination and source information for you.

    //     5.9 close() and shutdown()—Get outta my face!
    // Whew! You’ve been send()ing and recv()ing data all day long, and you’ve had it. You’re ready to close the connection on your socket descriptor. This is easy. You can just use the regular Unix file descriptor close() function:

    close(sockfd);
    // This will prevent any more reads and writes to the socket. Anyone attempting to read or write the socket on the remote end will receive an error.

    // Just in case you want a little more control over how the socket closes, you can use the shutdown() function. It allows you to cut off communication in a certain direction, or both ways (just like close() does). Synopsis:

    int shutdown(int sockfd, int how);
    // sockfd is the socket file descriptor you want to shutdown, and how is one of the following:

    // how	Effect
    // 0	Further receives are disallowed
    // 1	Further sends are disallowed
    // 2	Further sends and receives are disallowed (like close())
    // shutdown() returns 0 on success, and -1 on error (with errno set accordingly).

    // If you deign to use shutdown() on unconnected datagram sockets, it will simply make the socket unavailable for further send() and recv() calls (remember that you can use these if you connect() your datagram socket).

    // It’s important to note that shutdown() doesn’t actually close the file descriptor—it just changes its usability. To free a socket descriptor, you need to use close().

    // Nothing to it.

    // (Except to remember that if you’re using Windows and Winsock that you should call closesocket() instead of close().)

    //     5.10 getpeername()—Who are you?
    // This function is so easy.

    // It’s so easy, I almost didn’t give it its own section. But here it is anyway.

    // The function getpeername() will tell you who is at the other end of a connected stream socket. The synopsis:

    // #include <sys/socket.h>

    // int getpeername(int sockfd, struct sockaddr *addr, int *addrlen); used but outdated
    int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    // sockfd is the descriptor of the connected stream socket, addr is a pointer to a struct sockaddr (or a struct sockaddr_in) that will hold the information about the other side of the connection, and addrlen is a pointer to an int, that should be initialized to sizeof *addr or sizeof(struct sockaddr).

    // The function returns -1 on error and sets errno accordingly.

    // Once you have their address, you can use inet_ntop(), getnameinfo(), or gethostbyaddr() to print or get more information. No, you can’t get their login name. (Ok, ok. If the other computer is running an ident daemon, this is possible. This, however, is beyond the scope of this document. Check out RFC 141323 for more info.)

    // 5.11 gethostname()—Who am I?
    // Even easier than getpeername() is the function gethostname(). It returns the name of the computer that your program is running on. The name can then be used by getaddrinfo(), above, to determine the IP address of your local machine.

    // What could be more fun? I could think of a few things, but they don’t pertain to socket programming. Anyway, here’s the breakdown:

    // #include <unistd.h>

    int gethostname(char *hostname, size_t size); // this happens to already be modern
    // The arguments are simple: hostname is a pointer to an array of chars that will contain the hostname upon the function’s return, and size is the length in bytes of the hostname array.

    // The function returns 0 on successful completion, and -1 on error, setting errno as usual.
}
