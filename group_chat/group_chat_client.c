#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define LISTENQ 8

// thread arguments structure to store connection file descriptor
struct thread_arguments
{
   int conectfd;
};

// function to send messages
static void *sender(void *arg)
{
   struct thread_arguments *arg_sender = (struct thread_arguments *)arg;

   char buffer[BUFFER_SIZE] = {0};
   while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
   {
      send(arg_sender->conectfd, buffer, BUFFER_SIZE, 0);
   }
}

// function to take care of recieving messages
static void *reveiver(void *arg)
{
   struct thread_arguments *arg_receiver = (struct thread_arguments *)arg;

   char buffer[BUFFER_SIZE] = {0};

   while (recv(arg_receiver->conectfd, buffer, BUFFER_SIZE, 0) > 0)
   {
      printf("[RECV] : %s", buffer);
   }
}

int main(int argc, char **argv)
{
   if (argc != 3)
   {
      printf("Usage: %s <IP address of server> <Port Number of server>\n", argv[0]);
      exit(1);
   }
   pthread_t helper;
   struct thread_arguments *arg_sender, *arg_receiver;

   struct sockaddr_in server_addr;

   // creating sender socket
   int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_fd < 0)
   {
      perror("Error creating socket for sending\n");
      exit(1);
   }

   // initializing socket structure
   bzero(&server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = inet_addr(argv[1]);
   server_addr.sin_port = htons(atoi(argv[2]));

   if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
   {
      perror("Error connecting to server \n");
      exit(1);
   }
   printf("Connection succes\n");

   // for recieving messages
   arg_receiver = (struct thread_arguments *)malloc(sizeof(struct thread_arguments));
   arg_receiver->conectfd = socket_fd;
   pthread_create(&helper, NULL, reveiver, (void *)arg_receiver);
   pthread_detach(helper);

   // for sending messages
   arg_sender = (struct thread_arguments *)malloc(sizeof(struct thread_arguments));
   arg_sender->conectfd = socket_fd;
   pthread_create(&helper, NULL, sender, (void *)arg_sender);
   pthread_detach(helper);

   // preventing program from exiting
   while (3)
   {
   }
   exit(0);
}