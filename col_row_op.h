#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* tok;
const int count_cols(char* row);
const char* getfield(char* row, int col_num);
long long getlength(char *row, int col_num);