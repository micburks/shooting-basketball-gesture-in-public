
#ifndef SBGIP_SERVER_H_
#define SBGIP_SERVER_H_

/*
 * Often there will only be one server on a machine and that server will
 * handle multiple clients using fork() This routine consists of waiting
 * for a connection, accept()ing it, and fork()ing a child process to
 * handle it
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "file.h"
#include "headers.h"
#include "receive.h"
#include "send.h"

#define BACKLOG 10

typedef struct {

  char *ext;
  char *mediatype;

} extn;

/*
 * Possible media types
 */
extn extensions[] = {{"gif", "image/gif"},
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
                     {0, 0}};

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

#endif /* SBGIP_SERVER_H_ */
