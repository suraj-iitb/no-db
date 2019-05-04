#include "vector.h"
#include "encodings.h"
#include "col_row_op.h"
#include "linear_index.h"

void create_table(const char * file)
{

    struct MapList *head = NULL;

    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    char *row = NULL;
    size_t len = 0;
    ssize_t read;
    int num_cols = 0;
    long num_rows = 0;
    long long cumulative_length = 0, prev_len = 0;
    char *collist;
    char *col_from_index;

    vector row_locations; // is a array of strings
    //each string holds addresses of different fields for a row

    // vector_init(&row_locations);    

    while ((read = getline(&row, &len, fp)) != -1)
    {

  		// collist_map: stores whatever columns are actually there in the map right now
    	// Adaptive index builder, when new query comes checks if column is present in index
    	// if yes, search file based on index
    	// if not, augment the index ---> 
    	// get row based on 1st column index / vector row_loc : fseek maybe
    	// tok tok karke 4th column ka address maybe?
    	// add to maplist
        if(num_rows == 0)
        {
            //reading columns i.e header of csv file
            // collist = (char *)malloc(read * sizeof(char));
            collist = strdup(row); 
            num_cols = count_cols(row);
            // printf("Number cols: %d\nCharacters read: %ld\nCol list: %s\n",num_cols, read, collist);
            
            //find col name for index no in csv header. Note index no starts from 1
            int index_no = 1;
            char *tmp = strdup(collist);
            strcpy(row, collist);
            col_from_index = getfield(tmp, index_no);
            // printf("col %s at index %d\n", col_from_index, index_no);

        }
        num_rows++;

        cumulative_length = prev_len + cumulative_length;
        // vector_add(&row_locations, cumulative_length);
        append(&head, cumulative_length);
        
        prev_len = read;     
    }

    printf("\nColumns:\n%s\n", collist);
    printf("Number of columns: %d\n\n",num_cols);
    printf("Index list built so far on following columns: %s\n\n",col_from_index);
    
    printf("Row locations based on index build so far:\n");
    printList(head, 10); // we want start address of first 10 rows

    //set pointer of file to desired position and read rows
    fseek(fp, 0, SEEK_SET);
    
    fclose(fp);
}