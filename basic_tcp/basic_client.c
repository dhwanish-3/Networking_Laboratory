#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Usage: basic_client <IP address of the server>"); // Print an error message if the command-line argument is not provided correctly
        exit(1);                                                  // Exit the program with a non-zero status code
    }
    struct sockaddr_in server_addr;                    // Structure to hold server address information
    char sendline[BUFFER_SIZE], recvline[BUFFER_SIZE]; // Buffers for sending and receiving data

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket

    // initializing socket
    memset(&server_addr, 0, sizeof(server_addr));     // Clear the server address structure
    server_addr.sin_family = AF_INET;                 // Set the address family to IPv4
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Set the server IP address
    server_addr.sin_port = htons(SERVER_PORT);        // Set the server port number

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Connect to the server

    while (fgets(sendline, BUFFER_SIZE, stdin) != NULL) // Read input from the user
    {

        send(sockfd, sendline, strlen(sendline), 0); // Send the input to the server

        if (recv(sockfd, recvline, BUFFER_SIZE, 0) == 0) // Receive response from the server
        {
            perror("The server terminated prematurely"); // Print an error message if the server terminated prematurely
            exit(4);                                     // Exit the program with a non-zero status code
        }
        printf("%s", "String received from the server: "); // Print a message indicating that a string is received from the server
        fputs(recvline, stdout);                           // Print the received string
    }

    exit(0); // Exit the program with a status code of 0 => success
}