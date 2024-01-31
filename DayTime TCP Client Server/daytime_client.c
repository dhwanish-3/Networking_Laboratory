#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MAXLINE 1024
#define LISTENQ 10

typedef struct sockaddr SA;

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    int port;

    if (argc != 3)
    {
        printf("Usage: a.out <IPaddress> <port no.>"); // Print usage if incorrect number of arguments provided
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {                           // Creates a TCP Socket
        printf("Socket Error"); // Print error if socket creation fails
    }

    port = atoi(argv[2]); // ascii to integer conversion

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        // pton = presentatin to network
        printf("inet_pton error for %s", argv[1]); // Print error if IP address conversion fails
    }

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        // Establishes a TCP Connnection
        printf("Connect Error.\n"); // Print error if connection fails
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        // reads server reply.
        recvline[n] = 0;

        if (fputs(recvline, stdout) == EOF)
        {
            printf("fputs error.\n"); // Print error if writing to stdout fails
        }
    }

    if (n < 0)
    {
        printf("read error"); // Print error if reading from socket fails
    }
    return 0;
}