#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000
#define LISTENQ 10

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr, client_addr;
    // socket length for pass as argument to accept function
    socklen_t client_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // creating socket

    // socket structure initialization
    bzero(&server_addr, sizeof(server_addr));        // zero out the server address structure
    server_addr.sin_family = AF_INET;                // set address family to IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // bind to any available network interface
    server_addr.sin_port = htons(SERVER_PORT);       // set the server port number

    // binding address and port number to socket
    bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // start listen to incoming requests from client
    listen(socket_fd, LISTENQ);

    while (3) // infinite loop to accept multiple client connections
    {
        // accepting a request
        int connectfd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);

        while (recv(connectfd, buffer, BUFFER_SIZE, 0) > 0) // receive data from client
        {
            printf("%s", "String received from and resent to the client:");
            puts(buffer);
            // put thank you as response
            strcpy(buffer, "Thank you\n");
            send(connectfd, buffer, BUFFER_SIZE, 0); // send response to client
            bzero(&buffer, sizeof(buffer));
        }

        // closing the connection
        close(connectfd);
    }

    // close the listening socket
    close(socket_fd);
    exit(0);
}