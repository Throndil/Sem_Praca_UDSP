#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

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

int main() {

    Table t;
    initTable(&t);
    openTable(&t,"../Skuska_tab.txt" );
    //tableInformation(&t);
    char *arr1[3] = {
            "Name",
            "Surname",
            "TelephoneNumber"
    };
    char *arr2[3] = {
            "string",
            "string",
            "int"
    };
    char *arr3[3] = {
            "Jano",
            "Mkrva",
            "0917547258"
    };
    char *arr4[3] = {
            "Alo",
            "Pog",
            "4891678914"
    };
    //createTable("../Skuska", 3, arr1, arr2);
    //deleteTable("../Skuska_tab.txt");
    //addData("../Skuska_tab.txt", arr3, t.columns_count);
    //addData("../Skuska_tab.txt", arr4, t.columns_count);
    //deleteData("../Skuska_tab.txt",3);

    //sortTable(&t, "Name", (void*) sort);
    //printf("\n");
    //sortTable(&t, "TelephoneNumber", (void*) sortDouble, DOUBLE_TYPE);
    //printTable(&t);
    printDataWithSubstr(&t, "Alo");
  
    return 0;
}
