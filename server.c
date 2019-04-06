

/*
 * Often there will only be one server on a machine and that server will
 * handle multiple clients using fork() This routine consists of waiting
 * for a connection, accept()ing it, and fork()ing a child process to
 * handle it
 */

#include "server.h"

/*
 * for reaping zombie children
 */
void sigchld_handler(const int s) {

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

/*
 * return address whether IPv4 or IPv6
 */
void *get_in_addr(const struct sockaddr *sa) {

  if (sa->sa_family == AF_INET) {

    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*
 * handle connection
 */
int connection(const int fd) {

  char *file = receive(fd);
  respond(fd, file);
  close(fd);
  return 0;
}

/*
 * perform all socket methods
 * create child processes upon accept()ing connections
 */
int open_connection(char *port) {

  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {

    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  /*
   * loop through all results and bind to the first possible one
   */
  for (p = servinfo; p != NULL; p = p->ai_next) {

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {

      perror("server->socket%s\n");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {

      fprintf(stderr, "server: setsockopt\n");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {

      close(sockfd);
      perror("server->bind");
      continue;
    }

    break;
  }

  if (p == NULL) {

    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  /*
   * done with server info structure
   */
  freeaddrinfo(servinfo);

  if (listen(sockfd, BACKLOG) == -1) {

    fprintf(stderr, "server: listen\n");
    exit(1);
  }

  /*
   * reap all dead processes
   */
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {

    fprintf(stderr, "server: sigaction\n");
    exit(1);
  }

  /*
   * all set for connection
   */
  printf("server: waiting for connections on port %s...\n", port);

  /*
   * accept loop
   */
  while (1) {

    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

    if (new_fd == -1) {

      perror("server->accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);

    int pid = fork();
    if (pid == -1) {

      fprintf(stderr, "server: fork failed\n");
      exit(1);

    }

    /*
     * this is child process
     */
    else if (pid == 0) {

      /*
       * child does not need socket
       */
      close(sockfd);

      /*
       * apply communication loop
       */
      connection(new_fd);

      /*
       * exit child process
       */
      _exit(0);

    }

    else {

      /*
       * parent does not need connection fd
       */
      close(new_fd);
    }
  }

  close(sockfd);
  return 0;
}
