CC = gcc
CFLAGS = -Wall -O2
LIBS = -lmicrohttpd -lsqlite3

all: server

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LIBS)

clean:
	rm -f server
