/*
 * file.c
 */

#include "file.h"

/*
 * return size of file if opened -- -1 on unopened
 */
int f_open(const char* file) {
  int file_fd = open(file, O_RDONLY);
  if (file_fd == -1) {
    fprintf(stderr, "could not open %s\n", file);
    return -1;
  }

  return file_fd;
}

/*
 * return the size of a file -- -1 on error
 */
off_t f_size(const int file_fd) {
  struct stat stat_struct;
  if (fstat(file_fd, &stat_struct) == -1) {
    fprintf(stderr, "server: could not get file size\n");
    return -1;
  }

  return stat_struct.st_size;
}

/*
 * return the time of last modification -- -1 on error
 */
time_t f_last_mod(const int file_fd) {
  struct stat stat_struct;
  if (fstat(file_fd, &stat_struct) == -1) {
    fprintf(stderr, "server: could not get file last modification\n");
    return -1;
  }

  return stat_struct.st_mtime;
}

/*
 * return 1 on accessible, 0 on not
 */
int f_can_read(const char* file) {
  if (access(file, R_OK) == 0) {
    return 1;

  }

  else {
    perror("server->cannot access file\n");
    return 0;
  }
}
