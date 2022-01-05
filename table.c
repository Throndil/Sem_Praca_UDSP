#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include "table.h"

#define ROW_LENGTH 1024

static _Bool tableExists(const char* tableName) {
    if (access(tableName, F_OK) == 0) {
        return true;
    }
    return false;
};

static int numberOfRows(char* name) {
    int count = 0;
    char c;

    FILE *tab_file = fopen(name, "r");

    for (c = getc(tab_file); c != EOF; c = getc(tab_file))
        if (c == '\n')
            count++;

    fclose(tab_file);

    return count - 2;
}

static int numberOfColumns(char* name) {
    int i,count = 0;
    char columns[ROW_LENGTH];

    FILE *tab_file = fopen(name, "r");

    fgets(columns, ROW_LENGTH, tab_file);
    for (i = 0; *(columns + i) != '\n'; i++)
        if (*(columns + i) == ';')
            count++;

    fclose(tab_file);

    return count + 1;
}

static char** getColumnNames(char* name, int numberOfColumns) {
    char **columns;
    int i = 0;
    char str[ROW_LENGTH];
    char *w = NULL;
    columns = (char**)malloc(sizeof (char*) * numberOfColumns);

    FILE *tab_file = fopen(name, "r");

    fgets(str, ROW_LENGTH, tab_file);

    w = strtok(str, ";");
    while (w != NULL) {
        *(columns + i) = (char *)malloc(sizeof (char));
        strcpy(*(columns + i), w);
        w = strtok(NULL, ";");
        if (w != NULL)
            w[strcspn(w, "\n")] = 0;
        i++;
    }

    /*for (i = 0; i < numberOfColumns; i++)
        if(i + 1 < numberOfColumns)
            printf("%s\n", columns[i]);
        else
            printf("%s", columns[i]);*/

    fclose(tab_file);
    return columns;
}

static char** getTypes(char* name, int numberOfColumns) {
    char **types;
    char str[ROW_LENGTH];
    int i = 0;
    char *w = NULL;
    types = (char**)malloc(sizeof (char*) * numberOfColumns);

    FILE *tab_file = fopen(name, "r");

    fgets(str, ROW_LENGTH, tab_file);
    fgets(str, ROW_LENGTH, tab_file);

    w = strtok(str, ";");
    while (w != NULL) {
        *(types + i) = (char *)malloc(sizeof (char));
        strcpy(*(types + i), w);
        w = strtok(NULL, ";");
        if (w != NULL)
            w[strcspn(w, "\n")] = 0;
        i++;
    }

    /*for (i = 0; i < numberOfColumns; i++)
        if(i + 1 < numberOfColumns)
            printf("%s\n", types[i]);
        else
            printf("%s", types[i]);*/

    fclose(tab_file);
    return types;
}

static int getLastId(const char* tableName) {
    char str[1024];

    if(!tableExists(tableName)) {
        perror("Table does not exists");
        return 0;
    }

    FILE* tab_file = fopen(tableName, "r");

    fseek(tab_file, 0, SEEK_SET);

    while(!feof(tab_file))
    {
        memset(str, 0x00, 1024);
        fscanf(tab_file, "%[^\n]\n", str);
    }

    return atoi(strtok(str, ";"));
}

void initTable(Table* t) {
    t->row_count = 0;
    t->columns_count = 0;
    t->columnNames = NULL;
    t->types = NULL;
}

_Bool openTable(Table* t, char* name) {
    if (!tableExists(name)) {
        perror("Table does not exists.");
        return false;
    }

    t->row_count = numberOfRows(name);
    t->columns_count = numberOfColumns(name);
    t->columnNames = getColumnNames(name, t->columns_count);
    t->types = getTypes(name, t->columns_count);
    strcpy(t->name, name);

    return true;
}

void closeTable(Table* t) {
    for (int i = 0; i < t->columns_count; i ++) {
        free(t->columnNames[i]);
        free(t->types[i]);
    }
    free(t->columnNames);
    free(t->types);
    t->columnNames = NULL;
    t->types = NULL;
    t->row_count = 0;
    t->columns_count = 0;
    memset(t->name, 0, sizeof(t->name));
}

void tableInformation(Table* t) {
    int i;
    printf("Number of Rows: %d\n"
                  "Number of Columns: %d\n", t->row_count, t->columns_count);
    printf("Columns: ");
    for (i = 0; i < t->columns_count; i++)
        if(i + 1 < t->columns_count)
            printf("%s (%s),", t->columnNames[i], t->types[i]);
        else
            printf("%s (%s)\n", t->columnNames[i], t->types[i]);
}

_Bool createTable(char name[TABLE_NAME_SIZE], int num_columns, char** column_names, char** types) {
    int i;
    char fileName[TABLE_NAME_SIZE];
    strcpy(fileName, name);
    strcat(fileName, "_tab.txt");
    if (tableExists(fileName)) {
        perror("File already exists.");
        return false;
    }

    FILE* tab_file = fopen(fileName, "w");

    for (i = 0; i < num_columns;i++) {
        fputs(*(column_names+ i), tab_file);
        if (i + 1 == num_columns) {
            fputc('\n', tab_file);
        } else {
            fputc(';', tab_file);
        }
    }

    for (i = 0; i < num_columns;i++) {
        fputs(*(types + i), tab_file);
        if (i + 1 == num_columns) {
            fputc('\n', tab_file);
        } else {
            fputc(';', tab_file);
        }
    }

    fclose(tab_file);

    return true;
}

_Bool deleteTable(char* tableName) {
    if(!tableExists(tableName)) {
        perror("File does not exists");
        return false;
    }
    if(remove(tableName) == 0) {
        printf("Table deleted successfully.");
        return true;
    } else {
        perror("Unable to delete the table.");
        return false;
    }
}

_Bool addData(const char* tableName, char** data, int numberOfColumns) {
    int i;
    int id;
    char str[3];

    if(!tableExists(tableName)) {
        perror("Table does not exists");
        return false;
    }

    id = getLastId(tableName) + 1;
    sprintf(str, "%d", id);

    FILE* tab_file = fopen(tableName, "a");

    fputs(str, tab_file);
    fputs(";", tab_file);
    for (i = 0; i < numberOfColumns - 1; i++) {
        fputs(*(data + i), tab_file);
        if (i + 1 < numberOfColumns - 1) {
            fputs(";", tab_file);
        } else {
            fputs("\n", tab_file);
        }
    }

    fclose(tab_file);

    printf("Data added successfully\n");
    return true;
}

_Bool deleteData(const char* tableName, int id) {
    char *ret;
    char str[ROW_LENGTH];
    char copy[ROW_LENGTH];
    _Bool deleted = false;

    if(!tableExists(tableName)) {
        perror("Table does not exists");
        return false;
    }

    FILE* tab_file = fopen(tableName, "r");
    FILE* temp = fopen("temp.txt", "wb+");

    fgets(str, ROW_LENGTH, tab_file);
    fputs(str, temp);
    memset(str, 0, sizeof(str));
    fgets(str, ROW_LENGTH, tab_file);
    fputs(str, temp);
    memset(str, 0, sizeof(str));

    while (fgets(str, ROW_LENGTH, tab_file)) {
        strcpy(copy, str);
        ret = strtok(str, ";");

        if (atoi(ret) == id) {
            deleted = true;
            continue;
        } else {
            fprintf(temp, "%s", copy);
        }

        memset(str, 0, sizeof(str));
    }

    fclose(tab_file);
    fclose(temp);

    remove(tableName);
    rename("temp.txt", tableName);

    if (deleted) {
        printf("Data deleted successfully\n");
        return true;
    } else {
        perror("Data not found\n");
        return false;
    }
}

char* getTypeOfColumn(Table* t,const char* columnName) {
    for (int i = 0; i < t->columns_count; i++) {
        if (strcmp(columnName, t->columnNames[i]) == 0)
            return t->types[i];
    }
    printf("This column does not exist.");
    return NULL;
}

char* printTable(Table* t, char* dest) {
    char str[ROW_LENGTH];

    if(!tableExists(t->name)) {
        sprintf(dest, "%s", "Error.");
        return dest;
    }

    FILE* tab_file = fopen(t->name, "r");

    sprintf(dest + strlen(dest), "%s", "\n");

    while (fgets(str, ROW_LENGTH, tab_file) != NULL) {
        sprintf(dest + strlen(dest), "%s", str);
    }

    fclose(tab_file);

    printf("%s", dest);

    return dest;
}

char* printDataWithSubstr(Table* t, char* substr, char* dest) {
    char str[ROW_LENGTH];

    if(!tableExists(t->name)) {
        sprintf(dest + strlen(dest), "%s", "Error.");
        return dest;
    }

    FILE* tab_file = fopen(t->name, "r");

    sprintf(dest + strlen(dest), "%s", "\n");

    while (fgets(str, ROW_LENGTH, tab_file) != NULL) {
        if (strstr(str, substr) != NULL) {
            sprintf(dest + strlen(dest), "%s", str);
        }
    }

    fclose(tab_file);

    return dest;
}

char* sortTableAndPrint(Table* t, const char* column, void (*fun)(const void*,const void*), TYPE enumType, char* dest) {
    int i, index, columnIndex = 0, counter = 0;
    char** columns, **rows;
    char str[ROW_LENGTH], tempStr[ROW_LENGTH];
    char* w = NULL;
    int arrI[t->row_count];
    double arrD[t->row_count];
    rows = (char**)malloc(sizeof (char*) * t->row_count);
    columns = (char**)malloc(sizeof (char*) * t->row_count);

    for (i = 0; i < t->row_count; i++) {
        rows[i] = (char*)malloc(sizeof (str));
    }

    if(!tableExists(t->name)) {
        sprintf(dest + strlen(dest), "%s", "Error");
        return dest;
    }

    FILE* tab_file = fopen(t->name, "r");

    fgets(str, ROW_LENGTH, tab_file);
    fgets(str, ROW_LENGTH, tab_file);

    for (i = 0; i < t->columns_count; i++) {
        if (strcmp(t->columnNames[i], column) == 0)
            index = i;
    }

    i = 0;
    while (fgets(str, ROW_LENGTH, tab_file) != NULL) {
        w = strtok(str, ";");
        while (w != NULL) {
            if (index == i) {
                columns[columnIndex] = (char *)malloc(sizeof (char));
                strcpy(columns[columnIndex], w);
                columnIndex++;
            }
            w = strtok(NULL, ";");
            if (w != NULL)
                w[strcspn(w, "\n")] = 0;
            i++;
        }
        i = 0;
    }

    switch (enumType) {
        case CHAR_TYPE:
            for(char** col = columns; col < columns + t->columns_count; ++col)
                qsort(columns, t->row_count, sizeof (char *), (void*)fun);
            break;
        case INT_TYPE:
            for(i = 0; i < t->row_count; i++) {
                arrI[i] = atoi(columns[i]);
            }
            qsort(arrI, t->row_count, sizeof (int), (void*)fun);

            for(i = 0; i < t->row_count; i++) {
                sprintf(columns[i], "%d", arrI[i]);
            }
            break;
        case DOUBLE_TYPE:
            for(i = 0; i < t->row_count; i++) {
                arrD[i] = atof(columns[i]);
            }
            qsort(arrD, t->row_count, sizeof (double), (void*)fun);

            for(i = 0; i < t->row_count; i++) {
                sprintf(columns[i], "%.5f", arrD[i]);
            }
            break;
        default:
            break;
    }

    columnIndex = 0;
    memset(str, 0, sizeof(str));
    fseek(tab_file, 0, SEEK_SET);
    i = 0;
    while (fgets(str, ROW_LENGTH, tab_file) != NULL) {
        strcpy(tempStr, str);
        w = strtok(tempStr, ";");
        while (w != NULL) {
            if (strcmp(w, columns[i]) == 0 && columnIndex == index) {
                strcpy(rows[i], str);
                i++;
            }
            w = strtok(NULL, ";");
            if (w != NULL) {
                w[strcspn(w, "\n")] = 0;
                columnIndex++;
            }
            if (i == t->row_count)
                break;
        }
        columnIndex = 0;
        counter++;
        if (counter == t->row_count + 2) {
            counter = 0;
            fseek(tab_file, 0, SEEK_SET);
        }
        if (i == t->row_count)
            break;
        memset(str, 0, sizeof(str));

    }

    sprintf(dest + strlen(dest), "%s", "\n");
    for(i = 0; i < t->row_count; i++) {
        sprintf(dest + strlen(dest), "%s", rows[i]);
    }

    for(i = 0; i < t->row_count; i++) {
        free(rows[i]);
        free(columns[i]);
    }

    fclose(tab_file);
    free(rows);
    free(columns);

    return dest;
}