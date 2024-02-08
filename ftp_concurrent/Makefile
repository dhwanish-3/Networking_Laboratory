
CC = gcc
CFLAGS = -o

all: program

program: ftp_server ftp_client

ftp_server: server/ftp_server.c
	$(CC) $(CFLAGS) server/ftp_server server/ftp_server.c

ftp_client: client/ftp_client.c
	$(CC) $(CFLAGS) client/ftp_client client/ftp_client.c

clean:
	rm -f program server/ftp_server client/ftp_client
