#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define LISTENQ 8

struct thread_arguments
{
    int socket_fd;
    int connectfd;
    struct sockaddr client_addr;
};

struct thread_arguments *clients[10];
int client_count = 0;

// function to handle a single client
static void *handle_client(void *arg)
{
    // getting thread arguments
    struct thread_arguments *client = (struct thread_arguments *)arg;

    clients[client_count] = client;
    client_count++;
    
    char buffer[BUFFER_SIZE] = {0};
    while (recv(client->connectfd, buffer, BUFFER_SIZE, 0) > 0)
    {
        printf("String received from client : ");
        puts(buffer);
        for (int i = 0; i < client_count; i++)
        {
            // skipping sending to itself
            if (client->connectfd == clients[i]->connectfd)
            {
                continue;
            }
            send(clients[i]->connectfd, buffer, BUFFER_SIZE, 0);
        }
        // clearing buffer
        bzero(buffer, BUFFER_SIZE);
    }
    close(client->connectfd);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./%s <Port Number>\n", argv[0]);
        exit(1);
    }
    pthread_t helper;
    struct sockaddr_in server_addr, client_addr;
    socklen_t socket_len = sizeof(struct sockaddr);

    // creating socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error creating socket \n");
        exit(1);
    }
    printf("Socket created succesfully\n");

    // socket structure initialization
    bzero(&server_addr, socket_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // binding socket structure
    bind(socket_fd, (struct sockaddr *)&server_addr, socket_len);

    listen(socket_fd, LISTENQ); // start listening to client requests

    while (3)
    {
        int connectfd = accept(socket_fd, (struct sockaddr *)&client_addr, &socket_len);
        if (connectfd > 0)
        {
            struct thread_arguments *new_client = (struct thread_arguments *)malloc(sizeof(struct thread_arguments));
            memcpy(&(new_client->client_addr), &client_addr, socket_len);
            new_client->connectfd = connectfd;
            new_client->socket_fd = socket_fd;

            printf("Accepted request from %s and created thread to handle the client\n", inet_ntoa(client_addr.sin_addr));
            pthread_create(&helper, NULL, handle_client, (void *)new_client);
            pthread_detach(helper);
        }
    }
    exit(0);
}