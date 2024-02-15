#include <stdlib.h>     // Include the standard library header file
#include <stdio.h>      // Include the standard input/output header file
#include <sys/types.h>  // Include the types header file for system calls
#include <sys/socket.h> // Include the socket header file for socket-related functions
#include <netinet/in.h> // Include the internet header file for internet address structures
#include <string.h>     // Include the string header file for string manipulation functions
#include <unistd.h>     // Include the unistd header file for miscellaneous functions

#include <arpa/inet.h> // Include the internet header file for internet operations
#include <pthread.h>   // Include the pthread header file for thread-related functions

#define LISTENQ 8 // Define the maximum number of pending connections in the listen queue

struct thread_arguments // Define a structure for thread arguments
{
    char ip[15]; // IP address
    int port;    // Port number
};

// Function for sending messages to the server
static void *sender(void *arg)
{
    struct thread_arguments *args;         // Declare a pointer to the thread arguments structure
    args = (struct thread_arguments *)arg; // Cast the argument to the thread arguments structure

    int n, sockfd;               // Declare variables for the number of bytes read and the socket file descriptor
    struct sockaddr_in servaddr; // Declare a structure for the server address

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Create a TCP socket
    {
        perror("Problem in creating the socket"); // Print an error message if socket creation fails
        exit(2);                                  // Exit the program with an error code
    }

    memset(&servaddr, 0, sizeof(servaddr));         // Clear the server address structure
    servaddr.sin_family = AF_INET;                  // Set the address family to IPv4
    servaddr.sin_addr.s_addr = inet_addr(args->ip); // Set the IP address
    servaddr.sin_port = htons(args->port);          // Set the port number

    sleep(5);                                                                // Sleep for 5 seconds
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) // Connect to the server
    {
        perror("Problem in connecting to the server"); // Print an error message if connection fails
        exit(3);                                       // Exit the program with an error code
    }
    else
    {
        printf("Connection success\n"); // Print a success message if connection is successful
    }

    char buf[1024]; // Declare a buffer for storing the message

    while ((n = scanf("%[^\n]%*c", buf)) > 0)
    {                                          // Read a line of input from the user
        send(sockfd, buf, strlen(buf) + 1, 0); // Send the message to the server
    }
}

// Function for receiving messages from the client
static void *receiver(void *arg)
{
    struct thread_arguments *args;         // Declare a pointer to the thread arguments structure
    args = (struct thread_arguments *)arg; // Cast the argument to the thread arguments structure

    int n, listenfd, connfd;              // Declare variables for the number of bytes read, the listening socket file descriptor, and the connected socket file descriptor
    struct sockaddr_in cliaddr, servaddr; // Declare structures for the client and server addresses
    socklen_t clilen;                     // Declare a variable for the client address length

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Create a TCP socket for listening
    {
        perror("Problem in creating the socket"); // Print an error message if socket creation fails
        exit(2);                                  // Exit the program with an error code
    }

    servaddr.sin_family = AF_INET;                // Set the address family to IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Set the IP address to any available interface
    servaddr.sin_port = htons(args->port);        // Set the port number

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));  // Bind the socket to the server address
    listen(listenfd, LISTENQ);                                       // Start listening for incoming connections
    printf("Client is listening %d\n", args->port);                  // Print a message indicating that the client is listening on the specified port
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen); // Accept a connection from a client

    char buf[1024]; // Declare a buffer for storing the received message

    while ((n = recv(connfd, buf, 1024, 0)) > 0) // Receive a message from the client
    {
        printf("[RECV] %s\n", buf); // Print the received message
    }
}

int main(int argc, char **argv)
{
    int listenfd, sockfd, connfd;         // Declare variables for the listening socket file descriptor and the connected socket file descriptor
    struct sockaddr_in cliaddr, servaddr; // Declare structures for the client and server addresses
    pthread_t h1, h2;                     // Declare thread identifiers
    struct thread_arguments *args, *argr; // Declare pointers to the thread arguments structures
    socklen_t clilen;                     // Declare a variable for the client address length

    if (argc != 4) // Check if the number of command-line arguments is correct
    {
        printf("./program IP listenPort sendPort\n"); // Print the correct usage of the program
        return 0;                                     // Exit the program
    }
    else
    {
        printf("IP: %s, listenPort: %s, sendPort: %s\n", argv[1], argv[2], argv[3]); // Print the IP address, listen port, and send port
    }

    argr = (struct thread_arguments *)malloc(sizeof(struct thread_arguments)); // Allocate memory for the receiver thread arguments structure
    argr->port = atoi(argv[2]);                                                // Set the receiver port number
    pthread_create(&h1, NULL, receiver, (void *)argr);                         // Create the receiver thread
    pthread_detach(h1);                                                        // Detach the receiver thread

    sleep(10); // Sleep for 10 seconds

    args = (struct thread_arguments *)malloc(sizeof(struct thread_arguments)); // Allocate memory for the sender thread arguments structure
    strcpy(args->ip, argv[1]);                                                 // Set the sender IP address
    args->port = atoi(argv[3]);                                                // Set the sender port number
    pthread_create(&h2, NULL, sender, (void *)args);                           // Create the sender thread
    pthread_detach(h2);                                                        // Detach the sender thread

    while (1) // Infinite loop
    {
        int i = 0; // Dummy variable
    };

    return 0; // Exit the program
}
