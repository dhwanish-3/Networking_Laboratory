#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LISTENQ 8 // Define the maximum number of pending connections in the listen queue

struct thread_arguments // Define a structure for thread arguments
{
    char ip[15]; // IP address
    int port;    // Port number
};

// Function for sending messages to the server
static void *sender(void *arg)
{
    struct thread_arguments *args;         // a pointer to the thread arguments structure
    args = (struct thread_arguments *)arg; // Cast the argument to the thread arguments structure

    int n, sockfd;
    struct sockaddr_in servaddr; //  a structure for the server address

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Create a TCP socket
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    memset(&servaddr, 0, sizeof(servaddr));         // Clear the server address structure
    servaddr.sin_family = AF_INET;                  // Set the address family to IPv4
    servaddr.sin_addr.s_addr = inet_addr(args->ip); // Set the IP address
    servaddr.sin_port = htons(args->port);          // Set the port number

    sleep(5);                                                                // Sleep for 5 seconds
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) // Connect to the server
    {
        perror("Problem in connecting to the server");
        exit(3);
    }
    else
    {
        printf("Connection success\n");
    }

    char buf[1024]; //  a buffer for storing the message

    while ((n = scanf("%[^\n]%*c", buf)) > 0)
    {
        send(sockfd, buf, strlen(buf) + 1, 0); // Send the message to the server
    }
}

// Function for receiving messages from the client
static void *receiver(void *arg)
{
    struct thread_arguments *args;
    args = (struct thread_arguments *)arg; // Cast the argument to the thread arguments structure

    int n, listenfd, connfd;
    struct sockaddr_in cliaddr, servaddr;
    socklen_t clilen;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Create a TCP socket for listening
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    servaddr.sin_family = AF_INET;                // Set the address family to IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Set the IP address to any available interface
    servaddr.sin_port = htons(args->port);        // Set the port number

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); // Bind the socket to the server address
    listen(listenfd, LISTENQ);                                      // Start listening for incoming connections
    printf("Client is listening %d\n", args->port);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen); // Accept a connection from a client

    char buf[1024];

    while ((n = recv(connfd, buf, 1024, 0)) > 0) // Receive a message from the client
    {
        printf("[RECV] %s\n", buf);
    }
}

int main(int argc, char **argv)
{
    int listenfd, sockfd, connfd;
    struct sockaddr_in cliaddr, servaddr;
    pthread_t h1, h2;                     // thread identifiers
    struct thread_arguments *args, *argr; // pointers to the thread arguments structures
    socklen_t clilen;                     // a variable for the client address length

    if (argc != 4) // Check if the number of command-line arguments is correct
    {
        printf("./program IP listenPort sendPort\n"); // Print the correct usage of the program
        return 0;
    }
    else
    {
        printf("IP: %s, listenPort: %s, sendPort: %s\n", argv[1], argv[2], argv[3]);
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

    while (1) // Infinite loop to keep the program running
    {
    };

    return 0;
}
