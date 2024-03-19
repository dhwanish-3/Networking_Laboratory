#include <stdlib.h>     // standard library header file
#include <stdio.h>      // standard input/output header file
#include <sys/types.h>  // system types header file
#include <sys/socket.h> // socket header file
#include <arpa/inet.h>  // internet address header file
#include <string.h>     // string header file
#include <unistd.h>     // POSIX operating system API header file

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr;                 // structure to hold server address information
    socklen_t socket_len = sizeof(struct sockaddr); // variable to hold the length of the client address
    char buffer[BUFFER_SIZE];                       // buffer to hold data

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket

    bzero(&server_addr, sizeof(server_addr));        // fill server_addr with zeroes
    server_addr.sin_family = AF_INET;                // set the address family to AF_INET
    server_addr.sin_port = htons(SERVER_PORT);       // set the server port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // set the server IP address as IP of system(all interfaces)

    bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // bind the socket to the server address

    // receive the request
    while (3)
    {
        fgets(buffer, BUFFER_SIZE, stdin);                                                                   // read input from the user
        sendto(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, socket_len);              // send data to the server
        int len = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &socket_len); // receive data from the server
        printf("%s", "String received from the Server:");
        puts(buffer);
        bzero(buffer, BUFFER_SIZE); // clear the buffer
    }

    close(socket_fd); // close the socket
    exit(0);
}