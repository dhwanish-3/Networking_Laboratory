#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000
#define LISTENQ 8 /*maximum number of client connections */

int main(int argc, char **argv)
{
    int socket_fd, connectfd, n;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr, server_addr;
    socklen_t socket_len = sizeof(client_addr);

    // creation of the socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // preparation of the socket address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY tells to bind to IP address of every interface of the system
    server_addr.sin_port = htons(SERVER_PORT);

    // binding socket structure to socket
    bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(socket_fd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {
        connectfd = accept(socket_fd, (struct sockaddr *)&client_addr, &socket_len);
        printf("%s\n", "Received request...");

        while ((n = recv(connectfd, buffer, BUFFER_SIZE, 0)) > 0)
        {
            printf("%s", "String received from and resent to the client:");
            puts(buffer);
            send(connectfd, buffer, n, 0);
            bzero(&buffer, sizeof(buffer));
        }

        if (n < 0)
        {
            perror("Read error");
            exit(1);
        }
        close(connectfd);
    }
    // close listening socket
    close(socket_fd);
    exit(0);
}