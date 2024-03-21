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

#define BUFFER_SIZE 1024
#define LISTENQ 10

int main(int argc, char **argv)
{
    char recvline[BUFFER_SIZE + 1];
    struct sockaddr_in server_addr;

    if (argc != 3)
    {
        printf("Usage: ./a.out <IP Address> <Port number>");
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a TCP Socket
    if (sockfd < 0)
    {
        printf("Socket creation error\n");
    }

    // initialize the server address structure
    bzero(&server_addr, sizeof(server_addr)); // fills server_addr with zeros.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        // pton = presentatin to network
        printf("inet_pton error for %s", argv[1]);
    }

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (read(sockfd, recvline, BUFFER_SIZE) > 0)
    {
        fputs(recvline, stdout); // print the received data
    }
    return 0;
}