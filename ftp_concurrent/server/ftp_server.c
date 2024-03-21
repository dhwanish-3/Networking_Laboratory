#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SIZE 1024
#define PORT 8080

void send_file(FILE *fp, int socket_fd)
{
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(socket_fd, data, sizeof(data), 0) == -1) // Send data to the client socket
        {
            perror("Error in sending data");
            exit(1);
        }
        bzero(data, SIZE); // Clear the data buffer
    }
}

int main()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t socket_len;
    char buffer[SIZE];

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket for the server
    if (socket_fd < 0)
    {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available network interface
    server_addr.sin_port = htons(PORT);              // Set the PORT number

    bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Bind the socket to the server address

    listen(socket_fd, 10); // Listen for incoming connections

    socket_len = sizeof(client_addr);

    while (3)
    {
        // accept a connection
        int connectfd = accept(socket_fd, (struct sockaddr *)&client_addr, &socket_len); // Accept a new connection

        printf("Received request...");
        pid_t childpid;

        if ((childpid = fork()) == 0)
        { // if it’s 0, it’s the child process

            close(socket_fd); // Close the listening socket in the child process

            int n = recv(connectfd, buffer, SIZE, 0); // Receive the file name from the client
            FILE *fp = fopen(buffer, "r");            // Open the file in read mode
            if (fp == NULL)
            {
                perror("FILE does not exist");
                exit(2); // Exit the child process
            }
            send_file(fp, connectfd); // Send the file to the client
            printf("Data written in the text file.\n");
            exit(0);
        }
        // close socket of the server
        close(connectfd); // Close the client socket in the parent process
    }
    exit(0);
}