#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 4096
#define PORT 8080

void write_file(int sockfd, char filename[]) // Function to write received data to a file
{
    char buffer[SIZE]; // Buffer to store received data

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("Error in creating file.");
        exit(1);
    }
    long completed = 0;
    while (1) // Loop to receive data until the end
    {
        int n = recv(sockfd, buffer, SIZE, 0); // Receive data from the socket
        completed += n;
        printf("%ld bytes received...\n", completed);
        if (n <= 0)
        {
            return;
        }
        fprintf(fp, "%s", buffer); // Write the received data to the file
        bzero(buffer, SIZE);       // Clear the buffer
    }
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2) // Check if the number of command line arguments is incorrect
    {
        printf("Usage: %s <Server IP address>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server_addr; // Server address structure
    char filename[SIZE];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (sockfd < 0)                               // Check if socket creation failed
    {
        perror("Error in socket");
        exit(1);
    }
    printf("Server socket created. \n");

    bzero(&server_addr, sizeof(server_addr));         // Clear the server address structure
    server_addr.sin_family = AF_INET;                 // Set address family
    server_addr.sin_port = htons(PORT);               // Set PORT number
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Set server IP address

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // Connect to the server
    {
        perror("Error in Connecting");
        exit(1);
    }
    printf("Connected to server.\n");
    scanf("%s", filename); // Read filename from user

    send(sockfd, filename, SIZE, 0); // Send filename to the server

    printf("File name sent successfully. \n");
    write_file(sockfd, filename); // Write received data to file

    printf("Data written in the file successfully.\n");
    close(sockfd); // Close the socket

    exit(0);
}