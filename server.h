
#ifndef SBGIP_SERVER_H_
#define SBGIP_SERVER_H_


/*
 * Often there will only be one server on a machine and that server will
 * handle multiple clients using fork() This routine consists of waiting
 * for a connection, accept()ing it, and fork()ing a child process to
 * handle it 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "send.h"
#include "receive.h"
#include "headers.h"
#include "file.h"

#define BACKLOG 10



typedef struct {

    char *ext;
    char *mediatype;

} extn;



/*
 * Possible media types 
 */
extn extensions[] = {
    {"gif", "image/gif"},
    {"txt", "text/plain"},
    {"jpg", "image/jpg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"ico", "image/ico"},
    {"zip", "image/zip"},
    {"gz", "image/gz"},
    {"tar", "image/tar"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"php", "text/html"},
    {"pdf", "application/pdf"},
    {"zip", "application/octet-stream"},
    {"rar", "application/octet-stream"},
    {0, 0}
};



/*
 * for reaping zombie children 
 */
void sigchld_handler(const int s);



/*
 * return address whether IPv4 or IPv6 
 */
void *get_in_addr(const struct sockaddr *sa);



/*
 * handle connection 
 */
int connection(const int fd);



/*
 * perform all socket methods
 * create child processes upon accept()ing connections
 */
int open_connection(char *port);




/*
 * struct addrinfo { int ai_flags; int ai_family; int ai_socktype; int
 * ai_protocol; size_t ai_addrlen; struct sockaddr *ai_addr; char
 * *ai_cannonname;
 * 
 * struct addrinfo *ai_next; }; struct sockaddr { unsigned short
 * sa_family; char sa_data[14]; }; or struct sockaddr_in { short int
 * sin_family; unsigned short int sin_port; struct in_addr sin_addr;
 * unsigned char sin_zero[8]; };
 * 
 * IPv4 only struct in_addr { uint32_t s_addr; };
 * 
 * IPv6 only struct sockaddr_in6 { u_int16_t sin6_family; u_int16_t
 * sin6_port; u_int32_t sin6_flowinfo; struct in6_addr sin6_addr;
 * u_int32_t sin6_scope_id; };
 * 
 * struct in6_addr { unsigned char s6_addr[16]; };
 * 
 * struct sockaddr_storage { sa_family_t ss_family;
 * 
 * char __ss_pad1[_SS_PAD1SIZE]; int64_t __ss_align; char
 * __ss_pad2[_SS_PAD2SIZE]; }; 
 */

#endif /* SBGIP_SERVER_H_ */
