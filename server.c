#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "manager.h"
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define PORT 9001

static void actions(int socket, char* buffer) {
    manGetActions(buffer);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
    recv(socket, buffer, 1024, 0);
}

static void tableCreation(int socket, char* buffer) {
    char **columns;
    char **types;
    char tableName[40];
    int numberOfColumns;

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter the name: ");
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));

    recv(socket, buffer, 1024, 0);
    strcpy(tableName, buffer);

    bzero(buffer, sizeof (&buffer));

    sprintf(buffer, "%s", "Enter the number of columns: ");
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));

    recv(socket, buffer, 1024, 0);
    numberOfColumns = atoi(buffer) + 1;
    columns = (char**)malloc(sizeof (char*) * numberOfColumns);
    types = (char**)malloc(sizeof (char*) * numberOfColumns);

    columns[0] = (char*)malloc(sizeof (char));
    strcpy(columns[0], "ID");
    types[0] = (char*)malloc(sizeof (char));
    strcpy(types[0], "int");

    for (int i = 1; i < numberOfColumns; i++) {
        columns[i] = (char*)malloc(sizeof (char));
        types[i] = (char*)malloc(sizeof (char));
        bzero(buffer, sizeof (&buffer));
        sprintf(buffer, "Name of %d. column: ", i);

        send(socket, buffer, strlen(buffer), 0);
        bzero(buffer, sizeof (&buffer));

        recv(socket, buffer, 1024, 0);

        strcpy(columns[i], buffer);
        bzero(buffer, sizeof (&buffer));

        sprintf(buffer, "Type of %d. column: ", i);

        send(socket, buffer, strlen(buffer), 0);
        bzero(buffer, sizeof (&buffer));

        recv(socket, buffer, 1024, 0);
        strcpy(types[i], buffer);
    }
    bzero(buffer, sizeof (&buffer));

    manCreateTable(buffer,tableName, numberOfColumns, columns, types);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

int serverStart(){

    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];

    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", PORT);

    if(listen(sockfd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        printf("[-]Error in binding.\n");
    }

    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        if((childpid = fork()) == 0){
            close(sockfd);
            while(1){
                actions(newSocket, buffer);

                if (atoi(buffer) == 1) {
                    tableCreation(newSocket, buffer);
                }
                if(strcmp(buffer, ":exit") == 0){
                    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }else{
                    printf("%s", buffer);
                    bzero(buffer, BUFFER_SIZE);
                }
            }
        }
    }

    close(newSocket);
    return 0;
}