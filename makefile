CC=gcc
DEFS=-D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS=-Wall -g -std=c99 -pedantic $(DEFS)

.PHONY: all clean

all: server client

server: server.o #common.o
	$(CC) $^ -o $@

client: client.o #common.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o client server
	rm -f client.o server.o common.o