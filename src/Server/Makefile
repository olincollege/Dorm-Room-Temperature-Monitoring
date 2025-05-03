# Makefile for multi-file C web server

CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lmicrohttpd -lsqlite3

SRC = main.c http_server.c db_handle.c
OBJ = $(SRC:.c=.o)
OUT = server

.PHONY: all clean

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)
