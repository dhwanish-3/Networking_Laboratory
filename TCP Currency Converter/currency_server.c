#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 40     /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

int main(int argc, char **argv)
{
  int listenfd, connfd, n;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;

  // creation of the socket
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  listen(listenfd, LISTENQ);

  printf("%s\n", "Server running...waiting for connections.");

  for (;;)
  {

    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    printf("%s\n", "Received request...");

    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    {
      int i = 0;
      char currency[12];
      char value[20];
      int n = strlen(buf);

      while (buf[i] != ' ')
      {
        currency[i] = buf[i];
        i++;
      }
      int j = 0;
      while (j < n)
      {
        value[j] = buf[i];
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

    if (n < 0)
    {
      perror("Read error");
      exit(1);
    }
    close(connfd);
  }
  // close listening socket
  close(listenfd);
}
