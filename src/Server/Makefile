# Makefile for C web server

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror -O2
LIBS = -lmicrohttpd -lsqlite3

SRC = main.c http_server.c api_handler.c static_handler.c utils.c db_handle.c
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
