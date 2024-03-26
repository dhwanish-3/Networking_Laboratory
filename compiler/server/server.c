#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define LISTENQ 8

// error function
void error(const char *msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}

// function to recieve file
void recieve_file(int socket_fd, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        error("Error in opening file");
    }
    char buffer[BUFFER_SIZE] = {0};
    while (3)
    {
        int n = recv(socket_fd, buffer, BUFFER_SIZE, 0);
        if (n <= 0)
        {
            break;
        }
        fprintf(file, "%s", buffer);
        bzero(buffer, BUFFER_SIZE); // clear buffer
    }
    fclose(file); // close file descriptor
    printf("File recieved\n");
}

// function to compile a c file
void compile_file(char *filename)
{
    printf("%s\n", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        error("File not available for compilation");
    }

    char command[100] = {0};
    sprintf(command, "gcc %s -o %s", filename, "compiled"); // creating command to compile c file
    printf("%s\n", command);
    system(command); // executing the command
    fclose(file);
    printf("File compiled successfully\n");
}

// function to execute and save output of a c file
void execute_file(char *filename, char *output)
{
    // checking if file exists
    FILE *file = popen(filename, "r");
    if (file == NULL)
    {
        error("Error in opening file");
    }
    pclose(file);

    char command[100] = {0};
    sprintf(command, "./%s", filename); // command to run the executable
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        error("Error in executing file");
    }

    // output of the program is stored in fp
    char buffer[BUFFER_SIZE] = {0};
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) // reading from fp to buffer
    {
        strcat(output, buffer);     // concatinating from buffer to output
        bzero(buffer, BUFFER_SIZE); // clearing buffer
    }
    pclose(fp);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("Usage: ./client <Port Number>");
    }

    struct sockaddr_in server_addr;
    socklen_t socket_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = {0};

    // creating socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        error("Error in creating socket");
    }

    // socket structure initialization
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // binding socket
    bind(socket_fd, (struct sockaddr *)&server_addr, socket_len);

    // starting to listen client requests
    listen(socket_fd, LISTENQ);

    while (3)
    {
        int connectfd = accept(socket_fd, (struct sockaddr *)&server_addr, &socket_len);
        if (connectfd > 0)
        {
            // initializing timeout structure
            struct timeval timeout;
            timeout.tv_sec = 5; // after 10 seconds recv will timeout
            timeout.tv_usec = 0;

            // setting timeout to connection
            if (setsockopt(connectfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
            {
                perror("setsockopt failed\n");
            }
            printf("Accepted request from %s\n", inet_ntoa(server_addr.sin_addr));
            strcpy(buffer, "Enter filename : ");
            send(connectfd, buffer, BUFFER_SIZE, 0); // sending instruction to enter filename
            bzero(buffer, BUFFER_SIZE);
            recieve_file(connectfd, "recieved.c"); // recieving file from client
            sleep(1);
            compile_file("recieved.c");              // compiling the recieved file
            execute_file("compiled", buffer);        // executing the compiled file
            send(connectfd, buffer, BUFFER_SIZE, 0); // sending the output to client
            close(connectfd);
            printf("Closed connection\n");
        }
    }
    exit(0);
}