#include "manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "server.h"
#include "client.h"

static int sortChar(const void* data,const void* data2) {
    const char **a = (const char **)data;
    const char **b = (const char **)data2;
    return strcmp(*a, *b);
}

static int sortInt(const void* data,const void* data2) {
    const int *a = (const int *)data;
    const int *b = (const int *)data2;
    return *a  - *b;
}

static int sortDouble (const void* data,const void* data2) {
    const double *a = (const double *)data;
    const double *b = (const double *)data2;
    return (int)*a  - *b;
}

void startApp() {
    int choice = 0;

    printf("1. server\n");
    printf("2. client\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            serverStart();
            break;
        case 2:
            clientStart();
            break;
        default:
            return;
    }
}

char* manGetActions(char* dest) {
    sprintf(dest, "%s", "\n"
            "1. Create Table\n"
            "2. Open Table\n"
            "3. Exit\n");
    return dest;
}

char* manCreateTable(char* dest,char* tableName, int numberOfColumns, char** columns, char** types) {
    if (createTable(tableName, numberOfColumns, columns, types)) {
        sprintf(dest, "%s", "Table created successfully.");
    }
    return dest;
}