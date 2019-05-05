#include "linear_index.h"
#include "table.h"

long long no_of_indices = 0;

/*Add col_locations to MapList*/
void append(struct MapList** head_ref, long long new_data) 
{ 
    /* 1. allocate node */
    struct MapList *new_node = (struct MapList*) malloc(sizeof(struct MapList)); 
    struct MapList *last = *head_ref;  /* used in step 5*/
  
    /* 2. put in the data  */
    char new_data_string[ADDRESS_SIZE];

    EncodeLong(new_data, new_data_string);
    new_node->col_locations  = (char *)malloc(ADDRESS_SIZE * sizeof(char));
    strcpy(new_node->col_locations, new_data_string);

    new_node->next = NULL; 
  
    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL) 
    { 
       *head_ref = new_node; 
       return; 
    } 
  
    /* 5. Else traverse till the last node */
    while (last->next != NULL) 
        last = last->next; 
  
    /* 6. Change the next of last node */
    last->next = new_node; 
    return; 
} 

void printList(struct MapList *node, long long len) 
{ 

  while (node != NULL && len != 0) 
  { 
     printf(" %ld ", DecodeLong(node->col_locations)); 
     node = node->next; 
     len--;
  } 
  printf("\n\n");
} 


//Creating index on column requested by user. get_colno contains column no of column
void create_index(int get_colno, char *file){

  FILE *fp = fopen(file, "r");
  no_of_indices++;

  fseek(fp, 0, SEEK_SET);
  long long num_rows = 0;
  long long cumulative_length = 0, prev_len = 0;
  ssize_t read;
  char *tmp;
  
  char *row = NULL;
  size_t len;

  if(collist != NULL){
    printf("%s\n", collist);
    tmp = strdup(collist);
    printf("%s\n", tmp);
    char *column_name = getfield(tmp, get_colno);
    printf("%s\n", column_name);
    //Realloc to update col_from_index which contains column names for all indices
    col_from_index = (char *) realloc(col_from_index, strlen(col_from_index)+strlen(column_name)+1);
    printf("%s\n", collist);
    if (num_rows != 0)
      strcat(col_from_index,",");
    strcat(col_from_index,column_name);

    //Scanning file from beginning
    while ((read = getline(&row, &len, fp)) != -1)
    {   
        cumulative_length = prev_len + cumulative_length;

        char *tmp = strdup(row);
        struct MapList *last; 
        if(num_rows ==0)
            last = head;

        printf("Before: %ld\n",DecodeLong(last->col_locations));

        //Realloc col_locations to store column address of new column
        last->col_locations  = (char *)realloc(last->col_locations, no_of_indices * ADDRESS_SIZE * sizeof(char));
        char new_data_string[ADDRESS_SIZE];
        // bzero(last->col_locations+8,ADDRESS_SIZE);
        long newlen = cumulative_length+getlength(tmp, get_colno);
        printf("NEW LENGTH %ld\n ",newlen);
        int byteEncoded = EncodeLong(newlen, new_data_string);
        strcpy(last->col_locations+(no_of_indices-1)*ADDRESS_SIZE, new_data_string);
        printf("After: %ld  %ld \n",DecodeLong(last->col_locations),DecodeLong(last->col_locations+ADDRESS_SIZE));

        last = last->next;     
        
        prev_len = read;    
        num_rows++;

    }
  }    
}

//create index on first column
void create_initial_index(char * file)
{

    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    char *row = NULL;
    size_t len = 0;
    ssize_t read;
    int num_cols = 0;
    long num_rows = 0;
    long long cumulative_length = 0, prev_len = 0;
     

    while ((read = getline(&row, &len, fp)) != -1)
    {

        if(num_rows == 0)
        {
            //reading columns i.e header of csv file
            collist = strdup(row); 
            num_cols = count_cols(row);
            // printf("Number cols: %d\nCharacters read: %ld\nCol list: %s\n",num_cols, read, collist);
            
            //find col name for index no in csv header. Note index no starts from 1
            char *tmp = strdup(collist);
            strcpy(row, collist);
            col_from_index = getfield(tmp, 1);
            // tmp = strdup(col_from_index);
            printf("col %ld at index %d\n", col_from_index, 1);

        }
        num_rows++;

        cumulative_length = prev_len + cumulative_length;
        // vector_add(&row_locations, cumulative_length);
        append(&head, cumulative_length);
        
        prev_len = read;      
    }

    printf("\nColumns:\n%s\n", collist);
    printf("Number of columns: %d\n\n",num_cols);
    // printf("Index list built so far on following columns: %s\n\n", col_from_index);
    
    printf("Row locations based on index build so far:\n");
    printList(head, 10); // we want start address of first 10 rows
    
    fclose(fp);
}