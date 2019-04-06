//

struct addrinfo {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  size_t ai_addrlen;
  struct sockaddr *ai_addr;
  char *ai_cannonname;

  struct addrinfo *ai_next;
};

// load up a bit, then call
getaddrinfo();

struct sockaddr {
  unsigned short sa_family;
  char sa_data[14];
};
// or
struct sockaddr_in {
  short int sin_family;
  unsigned short int sin_port;
  struct in_addr sin_addr;
  unsigned char sin_zero[8];
};

// IPv4 only
struct in_addr {
  uint32_t s_addr;
};

// IPv6 only
struct sockaddr_in6 {
  u_int16_t sin6_family;
  u_int16_t sin6_port;
  u_int32_t sin6_flowinfo;
  struct in6_addr sin6_addr;
  u_int32_t sin6_scope_id;
};

struct in6_addr {
  unsigned char s6_addr[16];
};

struct sockaddr_storage {
  sa_family_t ss_family;

  char __ss_pad1[_SS_PAD1SIZE];
  int64_t __ss_align;
  char __ss_pad2[_SS_PAD2SIZE];
};
// check to see if ss_family is AF_INET or IF_INET6
// then cast it to a struct sockaddr_in or struct sockaddr_in6

struct sockaddr_in sa;   // IPv4
struct sockaddr_in6 sa6; // IPv6

// presentation to network function -- convert ip address to binary
// representation check to make sure these calls return a result greater than 0
// -1 = error
// 0 = address messed up
inet_pton(AF_INET, "192.0.2.1", &(sa.sin_addr));                // IPv4
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6

// network to presentation function -- convert binary to ip address
// IPv4
char ip4[INET_ADDRSTRLEN];
struct sockaddr_in sa; // ...loaded
inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
printf("IPv4 address is: %s\n", ip4);
// IPv6
char ip6[INET6_ADDRSTRLEN];
struct sockaddr_in6 sa6; // ...loaded
inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
printf("IPv6 address is: %s\n", ip6);

// not sure exactly
// for making it all ipv6
struct sockaddr_in sa;
struct sockaddr_in6 sa6;
sa.sin_addr.s_addr = INADDR_ANY; // use my IPv4 address
sa6.sin6_addr = in6addr_any;     // use my IPv6 address

struct in6_addr ia6 = IN6ADDR_ANY_INIT;

// also use
getaddrinfo();
getnameinfo();

//
// order of system calls
//

// getaddrinfo()
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int getaddrinfo(const char *node,    // e.g. "www.example.com" or IP
                const char *service, // e.g. "http" or port number
                const struct addrinfo *hints, struct addrinfo **res);
// example use -- listen on host's IP port 3490 (no listening or network setup)
int status;
struct addrinfo hints;
struct addrinfo *servinfo;

memset(&hints, 0, sizeof hints); // make sure struct is empty
hints.ai_family = AF_UNSPEC;     // don't care if v4 or v6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE;     // fill in my ip for me

if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}
// AI_PASSIVE flag tells the program to bind to the IP of the host it's running
// on to bind to a specific local IP address, drop the AI_PASSIVE and put an IP
// address in for the first argument to getaddrinfo()

// servinfo now points to a linked list of 1 or more struct addrinfos
// do everything until you dont' need servinfo anymore
freeaddrinfo(servinfo); // free the linked list

// socket()
#include <sys/socket.h>
#include <sys/types.h>
int socket(int domain, int type, int protocol);

// example use -- use results from getaddrinfo()
int s;
struct addrinfo hints, *res;
// do the lookup (fill out hints struct, use getaddrinfo())
// do error checking
// walk the 'res' linked list looking for valid entries instead of assuming
// first one is good
s = socket(res->ai_family, res->ai_socktype,
           res->ai_protocol); // return socket descriptor or -1 on error

// bind()
// associates the socket with a port on your local machine
// port number used by kernel to match incoming packet to a certain process's
// socket descriptor
#include <sys/socket.h>
#include <sys/types.h>
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

// example use -- after everything up through socket() call
bind(sockfd, res->ai_addr, res->ai_addrlen);
// bind() returns -1 on error
// in both cases (socket() and bind()), errno is set to the error's value
// also -- don't go underboard(1024) with port numbers. these are reserved
// unless superuser (can be up to 65535)

// if bind() fails because address is in use, wait for it to clear or allow
// program to reuse the port, like below
int yes = 1;
// lose the 'address already in use' error message
if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
  perror("setsockopt");
  exit(1);
}
// bind() final note -- sometimes you won't absolutely need to call it
// if connect()ing to remote machine and you don't care what your local port is
//      (as is case with telnet (only care about remote port)), you can simply
//      call connect()
// it'll check to see if socket is unbound and will bind() to an unused local
// port if necessary

// connect()
#include <sys/socket.h>
#include <sys/types.h>
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// example -- after call to socket (no call to bind in this example)
connect(sockfd, res->ai_addr, res->ai_addrlen);
// returns -1 on error and sets the variable errno

// listen()
// to handle incoming connections -- first listen(), then accept()
int listen(int sockfd, int backlog);
// backlog is the number of connections allowed on the incoming queue
// incoming connections wait in a queue until you accept() them (sometimes
// silently limited to 20, can maybe get away with 5 or 10)

// listen() returns -1 and sets errno on error

// need to call bind() before we call listen() so server is running on a
// specific port sequence of calls for incoming connections
getaddrinfo();
socket();
bind();
listen();
// accept(); -- tricky part

// accept()
// someone will try to connect() to your maching on a port you are listen()ing
// on their connection will be queued up waiting to be accept()ed and you tell
// it to get the pending connection this will return you a new socket file
// descriptor to use for this single connection the original one is still
// listening for more new connections the new one is ready to send() and recv()
#include <sys/socket.h>
#include <sys/types.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// addr will usually be a pointer to a local struct sockaddr_storage
// this is where the info about the incoming connection will go
//  (with it you can determine which host is calling you from which port)
// addrlen is local integer variable set to sizeof(struc sockaddr_storage) --
// limit of bytes to add to addr
//  also will be changed to the value of addrlen if fewer bytes are put in
// accept() return -1 and sets errno on error

// example call after listen()
addr_size = sizeof their_addr;
new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
// if you're only getting a single connection ever,
//  you can close() the listening sockfd to prevent more incoming connections on
//  the same port, if desired
// ready to communicate on socket descriptor new_fd

// send() and recv()
// can use for stream sockets or connected datagram sockets
//  (regular unconnected datagram sockets will need sendto() and recvfrom())
int send(int sockfd, const void *msg, int len, int flags);
// msg is a pointer to the data you want to send
// len is the length of the data in bytes
// set flags to 0 (see man page on send() from info concerning flags)

// example
char *msg = "lkjsdflak";
int len, bytes_sent;
len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
// send() returns the number of bytes actually sent out (may be less than told
// to send)
//  if value returned is less than len, it's up to you to send the rest of the
//  string
// if packet is small (< 1K or so), it will probably manage to send the whole
// thing at once -1 returned on error and errno set to error number

int recv(int sockfd, void *buf, int len, int flags);
// read into buf, len is max length of the buffer, flags can again be set to 0
// returns the number of bytes actually read into the buffer or -1 on error with
// errno set if recv() returns 0, then the remote side has closed the connection
// on you

// sendto() and recvfrom()
// for unconnected datagram sockets
// these sockets are not connected to a remote host
// we need to give the destination address before we send a packet
int sendto(int sockfd, const void *msg, int len, unsigned int flags,
           const struct sockaddr *to, socklen_t tolen);
// to is a pointer, probably to another struct sockaddr_in|struct
// sockaddr_in6|struct sockaddr_storage (casted at the last minute)
//  containes the destination IP address and port
// tolen is an int(deep-down) set to sizeof *to|sizeof(struct sockaddr_storage)
// destination addrewss structure will be gotten from
// getaddrinfo()|recvfrom()|filled out by hand returns the same as send()

int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
             struct sockaddr *from, int *fromlen);
// from is pointer to local struct sockaddr_storage filled with IP addreess and
// port of originating machine fromlen is pointer to local int initialized to
// sizeof *from|sizeof(struct sockaddr_storage) on return fromlen will contain
// the length of the address actually stored in from struct sockaddr_storage is
// used as the socket type because it will be big enough to store IPv4 or IPv6
// sockaddr

// if you connect() a datagram socket you can simply use send() and recv() for
// all transactions the socket itself is still datagram socket,
//  packets still use UDP, but socket interface will automatically add
//  destination and soource information for you

// close() and shutdown()
// regular Unix file descriptor close() function
close(sockfd);
// prevents any more reads and writes to the socket -- anyone reading or writing
// to it will receive an error

// shutdown() offers a little more control over how the socket closes
// allowing you to cut off communication in a certain direction or both ways
// (like close() does)
int shutdown(int sockfd, int how);
// how: -- 0 = further receives are disallowed
//      -- 1 = further sends are disallowed
//      -- 2 = both are disallowed (like close())
// returns 0 on success, -1 on error with errno set
// shutdown doesn't actually close the file descriptor -- it just changes
// usability to free the descriptor, use close()

// getpeername()
// will tell you who is at the other end of a connected stream socket
#include <sys/socket.h>
int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
// addr will hold the information about the other side of the connection
// returns -1 on error and sets errno
// once you have their address, you can use inet_ntop(), getnameinfo(), or
// gethostbyaddr() to print or get more info

// gethostname() -- who am I?
// returns the name of the computer that your program is running on
// the name can then be used by gethostbyname() to determine the IP address of
// your local machine
#include <unistd.h>
int gethostname(char *hostname, size_t size);
// hostname points to array of chars that will contain the hostname upon return
// size is the length in bytes of the hostname array
// returns 0 on success, -1 on error with errno set
