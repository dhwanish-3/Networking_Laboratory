
CC = gcc
CFLAGS = -o

all: program

program: currency_server currency_client

currency_server: currency_server.c
	$(CC) $(CFLAGS) currency_server currency_server.c

currency_client: currency_client.c
	$(CC) $(CFLAGS) currency_client currency_client.c

clean:
	rm -f program currency_server currency_client
