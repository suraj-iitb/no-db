#include "col_row_op.h"
#include "table.h"

/*find no of cols in a row*/
const int count_cols(char* row)
{
    vector_add(&allColNames_list, "---");
    const char* tok;
    int num_cols = 0;
    // allColNames_list = (char **)malloc(sizeof(char **));
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        // allColNames_list = (char *)malloc(sizeof)
        num_cols++;
        vector_add(&allColNames_list, tok);
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
        if (!--col_num){
            // printf("%s\n", tok);
            return tok;
        }
    }
    return NULL;
}


long long getlength(char *row, int col_num)
{
    int len = 0;
    const char* tok;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        len += strlen(tok)+1; 
        if (!--col_num)
            return len;
    }
    return len;

}

