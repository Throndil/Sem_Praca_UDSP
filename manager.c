#include "manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
                        "3. Delete Table\n"
                        "To exit app, type :exit\n");
    return dest;
}

char* manCreateTable(char* dest,char* tableName, int numberOfColumns, char** columns, char** types) {
    if (createTable(tableName, numberOfColumns, columns, types)) {
        sprintf(dest, "%s", "Table created successfully.");
    }
    return dest;
}

char* manOpenTable(char* dest, Table* t, char* name) {
    if (openTable(t, name) == true) {
        sprintf(dest, "%s (%s)", "Table opened successfully.", name);
    } else {
        sprintf(dest, "%s (%s)", "Table does not exist.", name);
    }

    return dest;
}

char* manDeleteTable(char* dest, char* name) {
    if (deleteTable(name)) {
        sprintf(dest, "%s", "Table deleted successfully.");
    } else {
        sprintf(dest, "%s", "Table does not exists.");
    }
    return dest;
}

char* manGetOpenedTableActions(char* dest) {
    sprintf(dest, "%s", "\n"
                        "1. Add data to table.\n"
                        "2. Delete data from table.\n"
                        "3. Print table.\n"
                        "4. Print data with substr from table. \n"
                        "5. Sort data by column and print table. \n"
                        "6. Close table. \n");
    return dest;
}

char* manAddData(char* dest, char* tableName, char** data, int numberOfColumns) {
    if (addData(tableName, data, numberOfColumns)) {
        sprintf(dest, "%s","Data added to table successfully.");
    } else {
        sprintf(dest, "%s","Data cannot be added.");
    }
    return dest;
}

char* manDeleteData(char* dest, char* tableName, int id) {
    if (deleteData(tableName, id)) {
        sprintf(dest, "%s","Data deleted successfully.");
    } else {
        sprintf(dest, "%s","Data cannot be added.");
    }
    return dest;
}

char* manPrintTable(char* dest, Table* t) {
    printTable(t, dest);
    return dest;
}

char* manPrintTableWithSubstr(char* dest, char* substr, Table* t) {
    printDataWithSubstr(t, substr, dest);
    return dest;
}

char* manSortTable(Table* t, const char* column, char* dest) {
    char type[TABLE_NAME_SIZE];

    strcpy(type, getTypeOfColumn(t, column));

    if (strcmp("int", type) == 0)
        sortTableAndPrint(t, column, (void*)sortInt, INT_TYPE, dest);
    else if (strcmp("double", type) == 0)
        sortTableAndPrint(t, column, (void*) sortDouble, DOUBLE_TYPE, dest);
    else if (strcmp("string", type) == 0)
        sortTableAndPrint(t, column, (void*) sortChar, CHAR_TYPE, dest);
    return dest;
}