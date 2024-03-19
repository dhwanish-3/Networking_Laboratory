
CC = gcc
CFLAGS = -o

all: program

program: udp_server udp_client

udp_server: udp_server.c
	$(CC) $(CFLAGS) udp_server udp_server.c

udp_client: udp_client.c
	$(CC) $(CFLAGS) udp_client udp_client.c

clean:
	rm -f program udp_server udp_client
