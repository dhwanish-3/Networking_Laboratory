#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        perror("Usage ./object_name <IP_Address> <Port_Number>\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    socklen_t socket_len = sizeof(struct sockaddr);

    // buffers for communication
    char sendline[BUFFER_SIZE], recvline[BUFFER_SIZE];

    // creating new socket
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        perror("Error creating socket\n");
        exit(1);
    }

    // initialization of socket structure
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    fgets(sendline, BUFFER_SIZE, stdin);

    if (sendto(socket_fd, sendline, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, socket_len) < 0)
    {
        perror("Could not send to server\n");
        exit(1);
    }
    // printing if response recieved from server without error
    if (recvfrom(socket_fd, recvline, BUFFER_SIZE, 0, NULL, &socket_len) > 0)
    {
        printf("Server replied : %s\n", recvline);
    }

    // clearing buffers for later usage
    bzero(&sendline, sizeof(sendline));
    bzero(&recvline, sizeof(recvline));

    close(socket_fd);

    exit(0);
}