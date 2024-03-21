#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_STATION_NUM 10
#define BUFFER_SIZE 1024
#define SERVER_PORT 3000

// total number of requests so far
int total_requests = 0;
// data array to store temperature and battery level from clients
int data[MAX_STATION_NUM][2];

// The arguments needed to send a response
struct thread_arguments
{
    struct sockaddr client_address;
    socklen_t client_length;

    // attributes to store information from client
    int station_index;
    int temperature;
    int battery_level;
};

// The entry point for the threads
static void *send_response(void *arg)
{
    char out[BUFFER_SIZE];
    struct thread_arguments *args;

    // Cast the arguments
    args = (struct thread_arguments *)arg;

    memset(&out, 0, BUFFER_SIZE);

    // Print the arguments
    printf("Station Index: %d\n", args->station_index);
    printf("Temperature: %d\n", args->temperature);
    printf("Batter Level: %d\n", args->battery_level);

    // Check if the battery level is low
    if (args->battery_level < 30)
    {
        printf("Battery level is low for station number : %d\n", args->station_index);
    }

    data[args->station_index][0] = args->temperature;
    data[args->station_index][1] = args->battery_level;

    // Calculating the average temperature
    int sum = 0;
    int count = 0;
    for (int i = 0; i < MAX_STATION_NUM; i++)
    {
        if (data[i][1] > 0)
        {
            sum += data[i][0];
            count++;
        }
    }
    printf("Average temperature: %.2f\n\n\n", (float)sum / count);
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    // sendto(socket_fd, out, BUFFER_SIZE, 0, &(args->client_address), args->client_length);
    close(socket_fd);

    // Free the memory used by the arguments
    // (Caller Allocates/Callee Frees)
    free(args);

    return NULL;
}

int main(void)
{
    // initialize the data array
    for (int i = 0; i < MAX_STATION_NUM; i++)
    {
        data[i][0] = 0;
        data[i][1] = 0;
    }
    char buffer[BUFFER_SIZE];
    pthread_t helper; // A thread to handle the response
    struct sockaddr_in address, client_address;
    struct thread_arguments *args;
    socklen_t client_length = sizeof(struct sockaddr);

    // create an UDP socket
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    // initialize the address of this host
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(SERVER_PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY); // In case the host has multiple

    // bind the socket to the address
    bind(socket_fd, (struct sockaddr *)&address, sizeof(struct sockaddr));

    // processing requests
    while (3)
    {
        // Receive the request
        recvfrom(socket_fd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&client_address, &client_length);

        total_requests++;
        printf("Received request from %s\n", inet_ntoa(client_address.sin_addr));
        printf("Request: %s\n", buffer);

        // buffer will buffer the format of "station_index temperature battery_level\n"
        // Parse the request and store the information
        int station_index;
        int temperature;
        int battery_level;
        sscanf(buffer, "%d %d %d", &station_index, &temperature, &battery_level);

        // Make a copy of the message and the client address
        // for the thread that will handle the response
        args = (struct thread_arguments *)malloc(sizeof(struct thread_arguments));
        args->station_index = station_index;
        args->temperature = temperature;
        args->battery_level = battery_level;
        memcpy(&(args->client_address), &client_address, client_length);
        args->client_length = client_length;

        // Create a thread to handle the response
        pthread_create(&helper, NULL, send_response, (void *)args);
        pthread_detach(helper);
    }

    close(socket_fd);
}
