
CC = gcc
CFLAGS = -o

all: program

program: tcp_thread_server tcp_thread_client

tcp_thread_server: tcp_thread_server.c
	$(CC) $(CFLAGS) tcp_thread_server tcp_thread_server.c -pthread

tcp_thread_client: tcp_thread_client.c
	$(CC) $(CFLAGS) tcp_thread_client tcp_thread_client.c

clean:
	rm -f program tcp_thread_server tcp_thread_client
