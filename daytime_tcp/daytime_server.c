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

typedef struct sockaddr SA;

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t socket_len = sizeof(struct sockaddr);
    char buffer[BUFFER_SIZE];
    time_t ticks;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // creates a TCP Socket

    // initialize the server address structure
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    bind(socket_fd, (SA *)&server_addr, sizeof(server_addr));
    printf("Server is waiting connection at port %d\t\n", atoi(argv[1]));
    listen(socket_fd, LISTENQ);

    for (;;)
    {
        int connectfd = accept(socket_fd, (SA *)&client_addr, &socket_len);
        printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buffer, sizeof(buffer)), ntohs(client_addr.sin_port));
        ticks = time(NULL); // get the current time
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks)); // format the time
        write(connectfd, buffer, strlen(buffer)); // send the time to the client
        close(connectfd);
    }
    return 0;
}