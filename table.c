#include "vector.h"
#include "encodings.h"
#include "col_row_op.h"
#include "linear_index.h"

void create_table(const char * file)
{

    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);  

    char *row = NULL;
    size_t len = 0;
    
    ssize_t read = getline(&row, &len, fp);
    if(read <= 0)
        exit(EXIT_FAILURE);
    
    //reading columns i.e header of csv file
    char *collist = strdup(row); 
    int num_cols = count_cols(row);    
    
    printf("\nColumns:\n%s\n", collist);
    printf("Number of columns: %d\n\n",num_cols);
    
    fclose(fp);
}

// void create_table(const char * file)
// {

//     struct MapList *head = NULL;

//     FILE* fp = fopen(file, "r");
//     if (fp == NULL)
//         exit(EXIT_FAILURE);
    
//     char *row = NULL;
//     size_t len = 0;
//     ssize_t read;
//     int num_cols = 0;
//     long num_rows = 0;
//     long long cumulative_length = 0, prev_len = 0;
//     char *collist;
//     char *col_from_index;

//     // vector row_locations; // is a array of strings and each string holds addresses of different fields for a row
//     // vector_init(&row_locations);    

//     while ((read = getline(&row, &len, fp)) != -1)
//     {

//         if(num_rows == 0)
//         {
//             //reading columns i.e header of csv file
//             collist = strdup(row); 
//             num_cols = count_cols(row);
//             // printf("Number cols: %d\nCharacters read: %ld\nCol list: %s\n",num_cols, read, collist);
            
//             //find col name for index no in csv header. Note index no starts from 1
//             char *tmp = strdup(collist);
//             strcpy(row, collist);
//             col_from_index = getfield(tmp, 1);
//             // printf("col %s at index %d\n", col_from_index, index_no);

//         }
//         num_rows++;

//         cumulative_length = prev_len + cumulative_length;
//         // vector_add(&row_locations, cumulative_length);
//         append(&head, cumulative_length);
        
//         prev_len = read;     
//     }

//     printf("\nColumns:\n%s\n", collist);
//     printf("Number of columns: %d\n\n",num_cols);
//     printf("Index list built so far on following columns: %s\n\n",col_from_index);
    
//     printf("Row locations based on index build so far:\n");
//     printList(head, 10); // we want start address of first 10 rows

//     //set pointer of file to desired position and read rows
//     fseek(fp, 0, SEEK_SET);
    
//     fclose(fp);
// }