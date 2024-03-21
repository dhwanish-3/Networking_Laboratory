#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERV_PORT 3000
#define LISTENQ 8 /*maximum number of client connections */

int main(int argc, char **argv)
{
  char buffer[BUFFER_SIZE];
  struct sockaddr_in client_addr, server_addr;
  socklen_t socket_len = sizeof(struct sockaddr);

  // creation of the socket
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    perror("Error creating socket\n");
    exit(1);
  }
  // preparation of the socket address
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERV_PORT);

  bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  listen(socket_fd, LISTENQ);

  printf("%s\n", "Server running...waiting for connections.");

  for (;;)
  {
    int connfd = accept(socket_fd, (struct sockaddr *)&client_addr, &socket_len);
    printf("%s\n", "Received request...");

    while (recv(connfd, buffer, BUFFER_SIZE, 0) > 0)
    {
      int i = 0;
      char currency[12];
      char value[20];
      int n = strlen(buffer);

      while (buffer[i] != ' ')
      {
        currency[i] = buffer[i];
        i++;
      }
      int j = 0;
      while (j < n)
      {
        value[j] = buffer[i];
        i++;
        j++;
      }
      int num = atoi(value);
      if (strcmp(currency, "Pounds") == 0)
      {
        sprintf(value, "%.2f\n", num * 105.53);
      }
      else if (strcmp(currency, "Dollar") == 0)
      {
        sprintf(value, "%.2f\n", num * 83.15);
      }
      else
      {
        sprintf(value, "Invalid currency\n");
      }
      send(connfd, value, n, 0);
    }

    close(connfd);
  }
  // close listening socket
  close(socket_fd);
}
