CC=gcc
CFLAGS=-c -Wall -O3
EXEC=server

all: main.o server.o send.o receive.o headers.o file.o
	$(CC) main.o server.o send.o receive.o headers.o file.o -o $(EXEC)

main.o: main.c
	$(CC) $(CFLAGS) main.c

server.o: server.c server.h send.h headers.h file.h
	$(CC) $(CFLAGS) server.c

send.o: send.c send.h headers.h file.h
	$(CC) $(CFLAGS) send.c

receive.o: receive.c receive.h headers.h
	$(CC) $(CFLAGS) receive.c

headers.o: headers.c headers.h
	$(CC) $(CFLAGS) headers.c

file.o: file.c file.h
	$(CC) $(CFLAGS) file.c

clean:
	rm -rf *o $(EXEC)
