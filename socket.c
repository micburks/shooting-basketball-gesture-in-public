
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);    // make sure struct is empty
    hints.ai_family = AF_UNSPEC;        // don't care if v4 or v6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;        // fill in my ip for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // AI_PASSIVE flag tells the program to bind to the IP of the host it's running on
    // to bind to a specific local IP address, drop the AI_PASSIVE and put an IP address in for the first argument to getaddrinfo()

    // servinfo now points to a linked list of 1 or more struct addrinfos
    // do everything until you dont' need servinfo anymore


    int s;
    struct addrinfo hints, *res;

    // walk the 'res' linked list looking for valid entries instead of assuming first one is good
    // return socket descriptor or -1 on error
    if((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        fprintf(stderr, "socket not connected: %s\n", gai_strerror(s));
        exit(1);
    }

    int b = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if(b == -1) {
        fprintf(stderr, "bind failed: %s\n", gai_strerror(b));
        exit(1);
    }

    int yes = 1;
    // lose the 'address already in use' error message
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    int c = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if(c == -1) {
        fprintf(stderr, "connect failed: %s\n", gai_strerror(c));
        exit(1);
    }

    int l = listen(sockfd, int backlog);
    if(l == -1) {
        fprintf(stderr, "listen failed: %s\n", gai_strerror(l));
        exit(1);
    }

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    int send(int sockfd, const void *msg, int len, int flags);

    char *msg = "lkjsdflak";
    int len, bytes_sent;
    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);

    close(sockfd);
    freeaddrinfo(servinfo); // free the linked list

    return 0;

}

