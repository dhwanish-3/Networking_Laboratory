
CC = gcc
CFLAGS = -o

all: program

program: basic_server basic_client

basic_server: basic_server.c
	$(CC) $(CFLAGS) basic_server basic_server.c

basic_client: basic_client.c
	$(CC) $(CFLAGS) basic_client basic_client.c

clean:
	rm -f program basic_server basic_client
