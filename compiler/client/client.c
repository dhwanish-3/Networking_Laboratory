#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// error function
void error(const char *msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}

// function to send file to server
void send_file(int socket_fd, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        error("Error in opening file");
    }
    char buffer[BUFFER_SIZE] = {0};

    // reading from file and sending to server
    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        if (send(socket_fd, buffer, sizeof(buffer), 0) < 0)
        {
            error("Error in sending file");
        }
        bzero(buffer, BUFFER_SIZE); // clear buffer
    }
    fclose(file); // closing the file
    printf("File sent successfully\n");
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        error("Usage: ./client <IP Address> <Port Number>");
    }
    struct sockaddr_in server_addr;
    socklen_t socket_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = {0};

    // creating socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        error("Error in creating socket");
    }

    // socket structure initialization
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // connecting to server
    if (connect(socket_fd, (struct sockaddr *)&server_addr, socket_len) < 0)
    {
        error("Error in connecting to server");
    }
    printf("Connected to server\n");

    // recieving prompt to enter filename from server
    recv(socket_fd, buffer, BUFFER_SIZE, 0);
    printf("%s\n", buffer);
    scanf("%s", buffer);          // getting filename
    send_file(socket_fd, buffer); // sending the file to server
    bzero(buffer, BUFFER_SIZE);   // clear buffer

    recv(socket_fd, buffer, BUFFER_SIZE, 0); // recieving output from server
    printf("output: %s\n", buffer);

    close(socket_fd);
    exit(0);
}