CC=gcc
CFLAGS=-g

all: server.c
	$(CC) $(CFLAGS) server.c -o server
