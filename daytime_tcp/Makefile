
CC = gcc
CFLAGS = -o

all: program

program: daytime_server daytime_client

daytime_server: daytime_server.c
	$(CC) $(CFLAGS) daytime_server daytime_server.c

daytime_client: daytime_client.c
	$(CC) $(CFLAGS) daytime_client daytime_client.c

clean:
	rm -f program daytime_server daytime_client
