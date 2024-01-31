#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections*/

int main(int argc, char **argv)
{
    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    // Create a socket for the soclet
    // If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // bind the socket
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // listen to the socket by creating a connection queue, then wait for clients
    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {

        clilen = sizeof(cliaddr);
        // accept a connection
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        printf("%s\n", "Received request...");

        if ((childpid = fork()) == 0)
        { // if it’s 0, it’s child process

            printf("%s\n", "Child created for dealing with client requests");

            // close listening socket
            close(listenfd);

            while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
            {
                printf("%s", "String received from and resent to the client:");
                puts(buf);
                int i = 0;

                while (i < n)
                {
                    if (buf[i] >= 'a' && buf[i] <= 'z')
                    {
                        buf[i] = (char)buf[i] - 32;
                    }
                    else if (buf[i] >= 'A' && buf[i] <= 'Z')
                    {
                        buf[i] = (char)buf[i] + 32;
                    }
                    i++;
                }
                if (strlen(buf) >= 3 && (buf[0] == 'b' && buf[1] == 'y' && buf[2] == 'e') || (buf[0] == 'B' && buf[1] == 'Y' && buf[2] == 'E'))
                {
                    close(connfd);
                    exit(0);
                }
                send(connfd, buf, n, 0);
            }

            if (n < 0)
            {
                printf("%s\n", "Read  error");
            }

            exit(0);
        }
        // close socket of the server
        close(connfd);
    }
}
