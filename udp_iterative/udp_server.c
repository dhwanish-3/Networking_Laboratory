#include <stdlib.h>     // standard library header file
#include <stdio.h>      // standard input/output header file
#include <sys/types.h>  // system types header file
#include <sys/socket.h> // socket header file
#include <arpa/inet.h>  // internet address header file
#include <string.h>     // string header file
#include <unistd.h>     // POSIX operating system API header file

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr;                 // structure to hold server address information
    socklen_t client_len = sizeof(struct sockaddr); // variable to hold the size of the client address structure
    char buffer[BUFFER_SIZE];                       // buffer to hold received data

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket

    bzero(&server_addr, sizeof(server_addr));        // clear the server address structure
    server_addr.sin_family = AF_INET;                // set the address family to AF_INET
    server_addr.sin_port = htons(SERVER_PORT);       // set the server port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // set the server IP addresses of all interfaces of the system

    // bind the socket to the address
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // receive the request
    while (3)
    {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &client_len); // receive data from the client
        printf("%s", "String received from the client:");
        puts(buffer);

        // put "Thank you" as response
        strcpy(buffer, "Thank you\n");
        sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, client_len); // Send the response to the client

        bzero(buffer, BUFFER_SIZE); // clear the buffer
    }

    close(sockfd); // close the socket
    exit(0);
}