#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1024   /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    // basic check of the arguments
    // additional checks can be inserted
    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server>");
        exit(1);
    }

    // Create a socket for the client
    // If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); // convert to big-endian order

    printf("Enter data to be sent to the server\n");
    printf("Format is <station_index> <temperature> <battery_level>\n");

    fgets(sendline, MAXLINE, stdin); // Send the station name to the server
    if (sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("sendto error");
        exit(4);
    }

    // Receive the response from the server
    // ssize_t recv_len = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
    // if (recv_len < 0)
    // {
    //     perror("recvfrom error");
    //     exit(5);
    // }
    // recvline[recv_len] = '\0'; // Null-terminate the received data
    // printf("Response received from the server: %s\n", recvline);

    close(sockfd);
    exit(0);
}