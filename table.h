#ifndef SEMESTRALNAPRACAUDSP_TABLE_H
#define SEMESTRALNAPRACAUDSP_TABLE_H

#define TABLE_NAME_SIZE 40

typedef enum {
    CHAR_TYPE,
    INT_TYPE,
    DOUBLE_TYPE
} TYPE;

typedef struct Table {
    int columns_count;
    int row_count;
    char **columnNames;
    char **types;
    char name[TABLE_NAME_SIZE];
} Table;

void initTable(Table* t);
_Bool openTable(Table* t, char* name);
void closeTable(Table* t);
_Bool createTable(char name[TABLE_NAME_SIZE], int num_columns, char** column_names, char** types);
_Bool deleteTable(char* name);
_Bool addData(const char* tableName, char** data, int numberOfColumns);
void tableInformation(Table* t);
_Bool deleteData(const char* tableName, int id);
char* getTypeOfColumn(Table* t, const char* columnName);
char* printTable(Table* t, char* dest);
char* printDataWithSubstr(Table* t, char* substr, char* dest);
char* sortTableAndPrint(Table* t, const char* column, void (*fun)(const void*,const void*), TYPE enumType, char* dest);

#endif //SEMESTRALNAPRACAUDSP_TABLE_H
