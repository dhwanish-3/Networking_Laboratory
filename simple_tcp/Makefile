
CC = gcc
CFLAGS = -o

all: program

program: simple_server simple_client

simple_server: simple_server.c
	$(CC) $(CFLAGS) simple_server simple_server.c

simple_client: simple_client.c
	$(CC) $(CFLAGS) simple_client simple_client.c

clean:
	rm -f program simple_server simple_client
