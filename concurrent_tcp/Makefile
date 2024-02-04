
CC = gcc
CFLAGS = -o

all: program

program: concurrent_server concurrent_client

concurrent_server: concurrent_server.c
	$(CC) $(CFLAGS) concurrent_server concurrent_server.c

concurrent_client: concurrent_client.c
	$(CC) $(CFLAGS) concurrent_client concurrent_client.c

clean:
	rm -f program concurrent_server concurrent_client
