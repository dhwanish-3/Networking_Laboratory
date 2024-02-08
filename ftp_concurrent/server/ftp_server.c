#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1) // Send data to the client socket
        {
            perror("Error in sending data");
            exit(1);
        }
        bzero(data, SIZE); // Clear the data buffer
    }
}

int main()
{
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket for the server
    if (sockfd < 0)
    {
        perror("Error in socket");
        exit(1);
    }
    printf("Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available network interface
    server_addr.sin_port = htons(port);              // Set the port number

    e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Bind the socket to the server address
    if (e < 0)
    {
        perror("Error in Binding");
        exit(1);
    }
    printf("Binding Successful.\n");

    e = listen(sockfd, 10); // Listen for incoming connections
    if (e == 0)
    {
        printf("Listening...\n");
    }
    else
    {
        perror("Error in Listening");
        exit(1);
    }
    addr_size = sizeof(new_addr);

    while (3)
    {
        // accept a connection
        new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size); // Accept a new connection

        printf("%s\n", "Received request...");
        pid_t childpid;

        if ((childpid = fork()) == 0)
        { // if it’s 0, it’s the child process

            // close listening socket
            close(sockfd); // Close the listening socket in the child process

            int n = recv(new_sock, buffer, SIZE, 0); // Receive the file name from the client
            FILE *fp = fopen(buffer, "r");           // Open the file in read mode
            if (fp == NULL)
            {
                perror("FILE does not exist");
                exit(2);
            }
            send_file(fp, new_sock); // Send the file to the client
            printf("Data written in the text file.\n");
            exit(0);
        }
        // close socket of the server
        close(new_sock); // Close the client socket in the parent process
    }
}