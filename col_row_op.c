#include "col_row_op.h"

/*find no of cols in a row*/
const int count_cols(char* row)
{
    const char* tok;
    int num_cols = 0;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        num_cols++;
    }
    // printf("Row::%d\n",strlen(row));
    return num_cols;
}

/*find col name corresponding to col no 'col_num'*/
const char* getfield(char* row, int col_num) // where col_num starts from 1
{
    const char* tok;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--col_num)
            return tok;
    }
    return NULL;
}
