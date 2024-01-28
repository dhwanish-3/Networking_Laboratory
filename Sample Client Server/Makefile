
CC = gcc
CFLAGS = -o

all: program

program: server client

server: server.c
	$(CC) $(CFLAGS) server server.c

client: client.c
	$(CC) $(CFLAGS) client client.c

clean:
	rm -f program server client
