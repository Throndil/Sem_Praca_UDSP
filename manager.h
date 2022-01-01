//
// Created by kille on 30. 12. 2021.
//

#ifndef SEMESTRALNAPRACAUDSP_MANAGER_H
#define SEMESTRALNAPRACAUDSP_MANAGER_H

#include "table.h"

void startApp();
char* manGetActions(char* dest);
char* manCreateTable(char* dest,char* tableName, int numberOfColumns, char** columns, char** types);
char* manDeleteTable(char* dest, char* name);
char* manOpenTable(char* dest, Table* t, char* name);
char* manGetOpenedTableActions(char* dest);
char* manAddData(char* dest, char* tableName, char** data, int numberOfColumns);
char* manDeleteData(char* dest, char* tableName, int id);
char* manPrintTable(char* dest, Table* t);
char* manPrintTableWithSubstr(char* dest, char* substr, Table* t);
char* manSortTable(Table* t, const char* column, char* dest);

#endif //SEMESTRALNAPRACAUDSP_MANAGER_H
