#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>
#include <fcntl.h> 
#include "course.h"
#include "student.h"
#include <errno.h>
#include "parser.h"

#define PORT 8080
#define BUFFER_SIZE 1024

pthread_mutex_t lock;

int output_file; 

void handle_client_request(int client_socket, char *buffer) {
    pthread_mutex_lock(&lock);  

    char log_buffer[BUFFER_SIZE];
    snprintf(log_buffer, sizeof(log_buffer), "Received request: %s\n", buffer);

    write(output_file, log_buffer, strlen(log_buffer));

    parse_request(buffer); 

    send(client_socket, "Operation successful", strlen("Operation successful"), 0);

    snprintf(log_buffer, sizeof(log_buffer), "Sent response: Operation successful\n");
    write(output_file, log_buffer, strlen(log_buffer));

    pthread_mutex_unlock(&lock);  
}

int main() {

    output_file = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(output_file < 0) {
        perror("Error opening log file.");
        exit(1);
    }

    int server_socket, client_socket, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    pthread_mutex_init(&lock, NULL);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_socket, 1) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    int active_client = 0;  

    while (1) {
   
        FD_ZERO(&readfds);

        FD_SET(server_socket, &readfds);

        int activity = select(server_socket + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error");
        }

        if (FD_ISSET(server_socket, &readfds)) {
            if (active_client == 0) {
                client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (client_socket < 0) {
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }

                printf("New client connected.\n");
                active_client = 1;  

                while ((valread = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
                    buffer[valread] = '\0';
                    handle_client_request(client_socket, buffer);
                }

                printf("Client disconnected.\n");
                close(client_socket);
                active_client = 0;  

            } else {
                int temp_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (temp_socket >= 0) {
                
                    char *message = "Server is busy, please try again later.\n";
                    send(temp_socket, message, strlen(message), 0);
                    close(temp_socket);
                    printf("Rejected new client, server is busy.\n");
                }
            }
        }
    }
    pthread_mutex_destroy(&lock);
    close(output_file); 
    return 0;
}
