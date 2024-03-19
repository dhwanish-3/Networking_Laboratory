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
    if (argc != 2)
    {
        perror("Usage: ./object_name <Port_number>\n");
        exit(1);
    }
    printf("Server running on Port Number : ");
    puts(argv[1]);

    struct sockaddr_in server_addr, client_addr;
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

    // socket structure initialization
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // binding the socket structure to socket descriptor
    bind(socket_fd, (struct sockaddr *)&server_addr, socket_len);

    while (3)
    {
        // when a successfull request is recieved
        if (recvfrom(socket_fd, recvline, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &socket_len) > 0)
        {
            int child_pid = fork(); // creating child process
            if (child_pid == 0)     // pid = 0 in child process
            {
                puts("\nChild process created...\n");
                printf("Recieved from client : %s\n", recvline);
                strcpy(sendline, "Thank you for the message\n");
                sendto(socket_fd, sendline, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, socket_len);
                close(socket_fd); // closing connection in child process
                exit(0); // exiting child
            }
        }
    }
    close(socket_fd);
    exit(0);
}