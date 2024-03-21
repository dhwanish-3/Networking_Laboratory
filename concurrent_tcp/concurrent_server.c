#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000 
#define LISTENQ 8

int main(int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_in cliaddr, servaddr;
    socklen_t socket_len = sizeof(struct sockaddr);

    // Create a socket for the soclet
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    // bind the socket
    bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // listen to the socket by creating a connection queue, then wait for clients
    listen(socket_fd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {
        // accept a connection
        int connectfd = accept(socket_fd, (struct sockaddr *)&cliaddr, &socket_len);

        printf("%s\n", "Received request...");

        int childpid = -1, n = 0;
        if ((childpid = fork()) == 0)
        { // if it’s 0, it’s child process

            printf("%s\n", "Child created for dealing with client requests");

            // close listening socket
            close(socket_fd);

            while ((n = recv(connectfd, buffer, BUFFER_SIZE, 0)) > 0)
            {
                printf("%s", "String received the client:");
                puts(buffer);
                int i = 0;

                while (i < n)
                {
                    if (buffer[i] >= 'a' && buffer[i] <= 'z')
                    {
                        buffer[i] = (char)buffer[i] - 32;
                    }
                    else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
                    {
                        buffer[i] = (char)buffer[i] + 32;
                    }
                    i++;
                }
                if (strlen(buffer) >= 3 && (buffer[0] == 'b' && buffer[1] == 'y' && buffer[2] == 'e') || (buffer[0] == 'B' && buffer[1] == 'Y' && buffer[2] == 'E'))
                {
                    close(connectfd);
                    exit(0);
                }
                send(connectfd, buffer, n, 0);
            }

            if (n < 0)
            {
                printf("%s\n", "Read  error");
            }

            exit(0);
        }
        // close socket of the server
        close(connectfd);
    }
    exit(0);
}
