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
#include "table.h"

#define BUFFER_SIZE 1024
#define PORT 9002

static void actions(int socket, char* buffer) {
    manGetActions(buffer);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
    recv(socket, buffer, 1024, 0);
}

static void tableCreation(int socket, char* buffer) {
    char **columns;
    char **types;
    char tableName[TABLE_NAME_SIZE];
    int numberOfColumns, i;

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter the name of table: ");
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
    strcpy(columns[0], "id");
    types[0] = (char*)malloc(sizeof (char));
    strcpy(types[0], "int");

    for (i = 1; i < numberOfColumns; i++) {
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

    for (i = 0; i < numberOfColumns; i++) {
        free(columns[i]);
        free(types[i]);
    }
    free(columns);
    free(types);
}

static void tableDelete(int socket, char* buffer) {
    char tableName[TABLE_NAME_SIZE];

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter the name of table: ");
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));

    recv(socket, buffer, 1024, 0);
    strtok(buffer, "txt");
    strcat(buffer, "tab.txt");
    strcpy(tableName, buffer);

    bzero(buffer, sizeof (&buffer));

    manDeleteTable(buffer, tableName);

    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

static void tableDeleteData(int socket, char* buffer, Table* t) {
    int id;

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter id of data to be deleted: ");
    send(socket, buffer, strlen(buffer), 0);

    bzero(buffer, sizeof (&buffer));
    recv(socket, buffer, 1024, 0);
    id = atoi(buffer);
    bzero(buffer, sizeof (&buffer));

    manDeleteData(buffer, t->name, id);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

static void tableAddData(int socket, char* buffer, Table* t) {
    char** data;
    int i;
    data = (char**)malloc(sizeof (char*) * (t->columns_count - 1));

    for (i = 0; i < t->columns_count - 1; i++) {
        bzero(buffer, sizeof (&buffer));
        data[i] = (char*)malloc(sizeof (char));
        sprintf(buffer, "%s %s (%s)", "Enter : ", t->columnNames[i + 1], t->types[i + 1]);
        send(socket, buffer, strlen(buffer), 0);
        bzero(buffer, sizeof (&buffer));
        recv(socket, buffer, 1024, 0);
        strcpy(data[i], buffer);
    }

    bzero(buffer, sizeof (&buffer));
    manAddData(buffer, t->name, data, t->columns_count);

    for (i = 0; i < t->columns_count - 1; i++) {
        free(data[i]);
    }
    free(data);

    send(socket, buffer, strlen(buffer), 0);
}

static void tablePrintTable(int socket, char* buffer, Table* t) {
    bzero(buffer, sizeof (&buffer));
    manPrintTable(buffer, t);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

static void tablePrintTableWithSubstr(int socket, char* buffer, Table* t) {
    char substr[BUFFER_SIZE];

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter substr: ");
    send(socket, buffer, strlen(buffer), 0);

    bzero(buffer, sizeof (&buffer));
    recv(socket, buffer, 1024, 0);
    strcpy(substr, buffer);
    bzero(buffer, sizeof (&buffer));

    manPrintTableWithSubstr(buffer, substr, t);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

static void tableSort(int socket, char* buffer, Table* t) {
    char columnName[BUFFER_SIZE];

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter the name of column to sort: ");
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));

    recv(socket, buffer, 1024, 0);
    strcpy(columnName, buffer);
    bzero(buffer, sizeof (&buffer));

    manSortTable(t, columnName, buffer);
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));
}

static void tableOpenedActions(int socket, char* buffer, Table* t) {
    int choice = 0;
    _Bool tableOpened = true;

    while(tableOpened) {
        bzero(buffer, sizeof (&buffer));
        manGetOpenedTableActions(buffer);
        send(socket, buffer, strlen(buffer), 0);

        bzero(buffer, sizeof (&buffer));
        recv(socket, buffer, 1024, 0);

        choice = atoi(buffer);

        switch (choice) {
            case 1:
                tableAddData(socket, buffer, t);
                break;
            case 2:
                tableDeleteData(socket, buffer, t);
                break;
            case 3:
                tablePrintTable(socket, buffer, t);
                break;
            case 4:
                tablePrintTableWithSubstr(socket, buffer, t);
                break;
            case 5:
                tableSort(socket, buffer, t);
                break;
            case 6:
                tableOpened = false;
                break;
            default:
                break;
        }
    }
}

static void tableOpening(int socket, char* buffer) {
    char name[TABLE_NAME_SIZE];

    Table t;
    initTable(&t);

    bzero(buffer, sizeof (&buffer));
    sprintf(buffer, "%s", "Enter the name of table: ");
    send(socket, buffer, strlen(buffer), 0);
    bzero(buffer, sizeof (&buffer));

    recv(socket, buffer, 1024, 0);
    strtok(buffer, "txt");
    strcat(buffer, "tab.txt");
    strcpy(name, buffer);

    bzero(buffer, sizeof (&buffer));
    manOpenTable(buffer, &t, name);

    send(socket, buffer, strlen(buffer), 0);

    tableOpenedActions(socket, buffer, &t);
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
                switch (atoi(buffer)) {
                    case 1:
                        tableCreation(newSocket, buffer);
                        break;
                    case 2:
                        tableOpening(newSocket, buffer);
                        break;
                    case 3:
                        tableDelete(newSocket, buffer);
                    default:
                        break;
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