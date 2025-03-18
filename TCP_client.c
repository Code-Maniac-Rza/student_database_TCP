#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "parser.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to the server successfully.\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    int operation = -1;
    char str1[256];

    while (fgets(str1, sizeof(str1), file) != NULL) {

        str1[strcspn(str1, "\n")] = 0;
        str1[strcspn(str1, "\r")] = 0;

        if (str1[0] == '#') {
            if (strstr(str1, "add student")) {
                operation = ADD_STUDENT;
            } else if (strstr(str1, "add course")) {
                operation = ADD_COURSE;
            } else if (strstr(str1, "modify student")) {
                operation = MODIFY_STUDENT;
            } else if (strstr(str1, "modify course")) {
                operation = MODIFY_COURSE;
            } else if (strstr(str1, "delete student")) {
                operation = DELETE_STUDENT;
            } else if (strstr(str1, "delete course")) {
                operation = DELETE_COURSE;
            }
            continue;
        }

        memset(message, 0, BUFFER_SIZE);
        char *tokens[4];
        int tokenCount = 0;
        char *token = strtok(str1, ", ");
        while (token != NULL && tokenCount < 4) {
            tokens[tokenCount++] = token;
            token = strtok(NULL, ", ");
        }

        switch (operation) {
            case ADD_STUDENT: {
                int rollno = atoi(tokens[0]);
                char *name = tokens[1];
                float cgpa = strtof(tokens[2], NULL);
                int num_courses = atoi(tokens[3]);
                snprintf(message, BUFFER_SIZE, "ADD_STUDENT %d %s %.2f %d", rollno, name, cgpa, num_courses);
                break;
            }
            case ADD_COURSE: {
                int rollno = atoi(tokens[0]);
                int code = atoi(tokens[1]);
                int mark = atoi(tokens[2]);
                snprintf(message, BUFFER_SIZE, "ADD_COURSE %d %d %d", rollno, code, mark);
                break;
            }
            case MODIFY_STUDENT: {
                int rollno = atoi(tokens[0]);
                float cgpa = strtof(tokens[1], NULL);
                snprintf(message, BUFFER_SIZE, "MODIFY_STUDENT %d %.2f", rollno, cgpa);
                break;
            }
            case MODIFY_COURSE: {
                int rollno = atoi(tokens[0]);
                int code = atoi(tokens[1]);
                int mark = atoi(tokens[2]);
                snprintf(message, BUFFER_SIZE, "MODIFY_COURSE %d %d %d", rollno, code, mark);
                break;
            }
            case DELETE_STUDENT: {
                int rollno = atoi(tokens[0]);
                snprintf(message, BUFFER_SIZE, "DELETE_STUDENT %d", rollno);
                break;
            }
            case DELETE_COURSE: {
                int rollno = atoi(tokens[0]);
                int code = atoi(tokens[1]);
                snprintf(message, BUFFER_SIZE, "DELETE_COURSE %d %d", rollno, code);
                break;
            }
            default:
                fprintf(stderr, "Unknown operation\n");
                break;
        }

        if (strlen(message) > 0) {
            printf("Sending: %s\n", message);
            send(sock, message, strlen(message), 0);

            int valread = read(sock, buffer, BUFFER_SIZE - 1);
            if (valread > 0) {
                buffer[valread] = '\0';
                printf("Server Response: %s\n", buffer);
            } else {
                printf("Failed to receive response from server.\n");
            }
        }
    }

    fclose(file);
    close(sock);
    return 0;
}