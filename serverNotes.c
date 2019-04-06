/*
 Often there will only be one server on a machine and that server
 will handle multiple clients using fork()
 This routine consists of waiting for a connection, accept()ing it, and
 fork()ing a child process to handle it
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

#define PORT "3490"
#define BACKLOG 10

typedef struct {

  char *ext;
  char *mediatype;

} extn;

typedef struct {

  char *version;
  char *status_code;
  char *server;
  char *last_modified;
  char *content_type;
  char *content_length;

} rspn_hdr;

/* Possible media types */
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

/* for reaping zombie children */
void sigchld_handler(const int s) {

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

/* return address whether IPv4 or IPv6 */
void *get_in_addr(const struct sockaddr *sa) {

  if (sa->sa_family == AF_INET) {

    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/* print error */
/* might actually fail because it is another call after error is met */
void error(const char *msg) {

  perror(msg);
  exit(1);
}

/* send a particalar message */
int send_msg(int fd, char *msg) {

  int m_len = strlen(msg);
  if (send(fd, msg, m_len, 0) == -1) {

    fprintf(stderr, "server: send failed");
  }

  return 0;
}

/* will be called for each file */
/* no return -- errors will be sent by subsequent functions */
int send_code(const int fd, char *code, const char *file) {

  printf("send code");

  int file_fd = f_open(file);
  if (file_fd == 0) {

    printf("code changed");
    code = "404";
  }

  if (strcmp(code, "200") == 0) {

    int file_size = f_size(file_fd);

    if (file_size == -1) {

      printf("no size");
      fprintf(stderr, "no size of fd: %d", file_fd);
      return -1;
    }

    char len_buffer[20];
    sprintf(len_buffer, "%d", file_size);

    char *content_length = "Content-Length: ";
    strcat(content_length, len_buffer);
    strcat(content_length, "\r\n\r\n");

    printf("send msgs 200");
    send_msg(fd, "HTTP/1.1 200 OK\r\n");
    send_msg(fd, "Server : I feel things...\r\n");
    send_msg(fd, "Last-Modified: Fri, 31 Dec 1999 23:59:59 GMT\r\n");
    send_msg(fd, "Content-Type: text/html\r\n");
    send_msg(fd, content_length);
    send_file(fd, file_fd, file_size);

  }

  else if (strcmp(code, "404") == 0) {

    char *msg404 =
        "<html><head><title>404 Not Found</head></title><body><p>404 Not "
        "Found: The requested resource could not be found!</p></body></html>";
    int len = strlen(msg404);
    char len_buffer[20];
    sprintf(len_buffer, "%d", len);

    char *content_length = "Content-Length: ";
    strcat(content_length, len_buffer);
    strcat(content_length, "\r\n\r\n");

    printf("send msgs 404");
    send_msg(fd, "HTTP/1.1 404 Not Found\r\n");
    send_msg(fd, "Server : I feel things...\r\n");
    send_msg(fd, "Last-Modified: Fri, 31 Dec 1999 23:59:59 GMT\r\n");
    send_msg(fd, "Content-Type: text/html\r\n");
    send_msg(fd, content_length);
    send_msg(fd, msg404);

    return 0;
  }

  return 0;
}

/* send an entire file */
int send_file(const int fd, const int file_fd, int file_size) {

  printf("send file");
  size_t total_bytes_sent = 0;
  ssize_t bytes_sent;
  while (total_bytes_sent < file_size) {
    printf("while send loop\n");

    if (bytes_sent =
            sendfile(fd, file_fd, 0, file_size - total_bytes_sent) <= 0) {

      if (errno == EINTR || errno == EAGAIN) {

        continue;
      }

      perror("server->sendfile");
      return -1;
    }

    total_bytes_sent += bytes_sent;
  }

  /* never reached? */
  /* perror will say success and return -1 */
  printf("post loop");
  return 0;
}

/* return size of file if opened -- 0 on unopened, -1 on size error */
int f_open(const char *file) {

  int file_fd = open(file, O_RDONLY);
  if (file_fd == -1) {

    fprintf(stderr, "could not open %s", file);
    return 0;
  }

  return file_fd;
}

/* return the size of a file -- -1 on error */
int f_size(const int file_fd) {

  struct stat stat_struct;
  if (fstat(file_fd, &stat_struct) == -1) {

    fprintf(stderr, "server: open error");
    return -1;
  }

  return (int)stat_struct.st_size;
}

/* handle connection */
int connection(const int fd) {

  fprintf(stdout, "connection");
  char *file = "index.html";
  send_code(fd, "200", file);
  close(fd);
  return 0;
}

int main(int argc, char **argv) {

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

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {

    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  /* loop through all results and bind to the first possible one */
  for (p = servinfo; p != NULL; p = p->ai_next) {

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {

      perror("server->socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {

      error("server: setsockopt");
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

  /* done with server info structure */
  freeaddrinfo(servinfo);

  if (listen(sockfd, BACKLOG) == -1) {

    error("server: listen");
  }

  /* reap all dead processes */
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {

    error("server: sigaction");
  }

  /* all set for connection */
  printf("server: waiting for connections...\n");

  /* accept loop */
  while (1) {

    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

    if (new_fd == -1) {

      perror("server->accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf("server: got connection from %s\n", s);

    int pid = fork();
    if (pid == -1) {

      printf("what the fork");
      error("server: fork failed");

    }

    /* this is child process */
    else if (pid == 0) {

      printf("child");
      /* child does not need socket */
      close(sockfd);

      /* apply communication loop */
      connection(new_fd);

      /* exit child process */
      _exit(0);

    }

    else {

      printf("parent\n");
      close(new_fd);
    }

    /* parent does not need connection fd */
    // printf ("parent");
    // close (new_fd);

    /* i don't know what this would be
    if (for_some_reason)
    {
        break;
    }
    */
  }

  close(sockfd);
  return 0;
}

/*
struct addrinfo {
    int             ai_flags;
    int             ai_family;
    int             ai_socktype;
    int             ai_protocol;
    size_t          ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_cannonname;

    struct addrinfo *ai_next;
};
struct sockaddr {
    unsigned short  sa_family;
    char            sa_data[14];
};
// or
struct sockaddr_in {
    short int           sin_family;
    unsigned short int  sin_port;
    struct in_addr      sin_addr;
    unsigned char       sin_zero[8];
};

//IPv4 only
struct in_addr {
    uint32_t s_addr;
};

//IPv6 only
struct sockaddr_in6 {
    u_int16_t       sin6_family;
    u_int16_t       sin6_port;
    u_int32_t       sin6_flowinfo;
    struct in6_addr sin6_addr;
    u_int32_t       sin6_scope_id;
};

struct in6_addr {
    unsigned char   s6_addr[16];
};

struct sockaddr_storage {
    sa_family_t ss_family;

    char        __ss_pad1[_SS_PAD1SIZE];
    int64_t     __ss_align;
    char        __ss_pad2[_SS_PAD2SIZE];
};
*/
