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
    socklen_t client_len = sizeof(struct sockaddr); // Declare a variable to hold the length of the client address
    char buffer[BUFFER_SIZE];                       // Declare a buffer to hold data

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket

    bzero(&server_addr, sizeof(server_addr));        // Set the server address structure to zero
    server_addr.sin_family = AF_INET;                // Set the address family to AF_INET
    server_addr.sin_port = htons(SERVER_PORT);       // Set the server port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Set the server IP address

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Bind the socket to the server address

    // receive the request
    while (3)
    {
        fgets(buffer, BUFFER_SIZE, stdin);                                                                // Read input from the user
        sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, client_len);              // Send data to the server
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &client_len); // Receive data from the server
        printf("%s", "String received from the Server:");                                                 // Print a message
        puts(buffer);                                                                                     // Print the received data
        bzero(buffer, BUFFER_SIZE);                                                                       // Clear the buffer
    }

    close(sockfd); // Close the socket
}