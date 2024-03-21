#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 3000

int main(int argc, char **argv)
{
   struct sockaddr_in servaddr;
   char sendline[BUFFER_SIZE], recvline[BUFFER_SIZE];

   if (argc != 2)
   {
      perror("Usage: ./TCPClient <IP address of the server>");
      exit(1);
   }

   // create a socket for the client
   int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_fd < 0)
   {
      perror("Problem in creating the socket");
      exit(2);
   }

   // creation of the socket
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(argv[1]);
   servaddr.sin_port = htons(SERVER_PORT); // convert to big-endian order which is network standard

   // Connection of the client to the socket
   if (connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
   {
      perror("Problem in connecting to the server");
      exit(3);
   }

   while (fgets(sendline, BUFFER_SIZE, stdin) != NULL)
   {
      send(socket_fd, sendline, strlen(sendline), 0);

      if (recv(socket_fd, recvline, BUFFER_SIZE, 0) == 0)
      {
         perror("The server terminated prematurely");
         exit(4);
      }
      printf("%s", "String received from the server: ");
      fputs(recvline, stdout);

      // clearing buffers
      memset(&recvline, 0, sizeof(recvline));
      memset(&sendline, 0, sizeof(sendline));
   }

   exit(0);
}