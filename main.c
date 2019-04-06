
/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>

int open_connection();

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: server <PORT>\n");
    exit(1);
  }

  open_connection(argv[1]);

  return 0;
}
