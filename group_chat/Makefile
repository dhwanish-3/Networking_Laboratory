
CC = gcc
CFLAGS = -o

all: program

program: group_chat_server group_chat_client

group_chat_server: group_chat_server.c
	$(CC) $(CFLAGS) group_chat_server group_chat_server.c -pthread

group_chat_client: group_chat_client.c
	$(CC) $(CFLAGS) group_chat_client group_chat_client.c -pthread

clean:
	rm -f program group_chat_server group_chat_client
