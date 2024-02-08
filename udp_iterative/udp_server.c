#include <stdlib.h>     // Include the standard library header file
#include <stdio.h>      // Include the standard input/output header file
#include <sys/types.h>  // Include the system types header file
#include <sys/socket.h> // Include the socket header file
#include <arpa/inet.h>  // Include the internet address header file
#include <string.h>     // Include the string header file
#include <unistd.h>     // Include the POSIX operating system API header file

#define BUFFER_SIZE 1024 // Define the buffer size as 1024
#define SERVER_PORT 3000 // Define the server port as 3000

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr;                 // Declare a structure to hold server address information
    socklen_t client_len = sizeof(struct sockaddr); // Declare a variable to hold the size of the client address structure
    char buffer[BUFFER_SIZE];                       // Declare a buffer to hold received data

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket

    bzero(&server_addr, sizeof(server_addr));        // Clear the server address structure
    server_addr.sin_family = AF_INET;                // Set the address family to AF_INET
    server_addr.sin_port = htons(SERVER_PORT);       // Set the server port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Set the server IP address

    // Bind the socket to the address
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive the request
    while (3)
    {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &client_len); // Receive data from the client
        printf("%s", "String received from the client:");                                                 // Print a message indicating that a string has been received
        puts(buffer);                                                                                     // Print the received string

        // Put "Thank you" as response
        strcpy(buffer, "Thank you\n");                                                       // Copy the response message to the buffer
        sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, client_len); // Send the response to the client

        bzero(buffer, BUFFER_SIZE); // Clear the buffer
    }

    close(sockfd); // Close the socket
}