#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "vector.h"
#define ADDRESS_SIZE sizeof(long long)
typedef char byte;
typedef union {
    long long ll;
    byte bytes[8];
} LongBytes;
typedef struct MapList {
    char *col_locations;
    struct MapList *next;
} MapList;
/*
 Naive encoding of a 64-bit long to 8 bytes. 
 */
int EncodeLong(long long l, byte *bytes) {
    LongBytes lb;
    lb.ll = l;
    memcpy(bytes, lb.bytes, 8);
    return 8;
}
/*
  Translates 8 bytes pointed to by 'bytes' to a 64-bit long.
 */
long long DecodeLong(byte *bytes) {
    LongBytes lb;
    memcpy(lb.bytes, bytes, 8);
    return lb.ll;
}
/* vector initialization*/
void vector_init(vector *v)
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}
/*vector count*/
int vector_count(vector *v)
{
    return v->count;
}
/*add  a row to vector*/
void vector_add(vector *v, void *e)
{
    if (v->size == 0) {
        v->size = 10;
        v->data = calloc(sizeof(void*), v->size);
        // memset(v->data, '\0', sizeof(void) * v->size);
    }
    // condition to increase v->data:
    // last slot exhausted
    if (v->size == v->count) {
        v->size *= 2; 
        v->data = realloc(v->data, sizeof(void*) * v->size);
    }
    v->data[v->count] = e;
    v->count++;
}
/*update a row in vector*/
void vector_set(vector *v, int index, void *e)
{
    if (index >= v->count) {
        return;
    }
    v->data[index] = e;
}
/* get a row at index 'index' from vector*/
void *vector_get(vector *v, int index)
{
    if (index >= v->count) {
        return;
    }
    return v->data[index];
}
/*delete a row from vector*/
void vector_delete(vector *v, int index)
{
    if (index >= v->count) {
        return;
    }
    v->data[index] = NULL;
    int i, j;
    void **newarr = (void**)malloc(sizeof(void*) * v->count * 2); // why 2?
    for (i = 0, j = 0; i < v->count; i++) {
        if (v->data[i] != NULL) {
            newarr[j] = v->data[i];
            j++;
        }       
    }
    free(v->data);
    v->data = newarr;
    v->count--;
}
void vector_free(vector *v)
{
    free(v->data);
}
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



/*Add col_locations to MapList*/
void append(struct MapList** head_ref, long long new_data) 
{ 
    /* 1. allocate node */
    struct MapList *new_node = (struct MapList*) malloc(sizeof(struct MapList)); 
    struct MapList *last = *head_ref;  /* used in step 5*/
  
    /* 2. put in the data  */
    char new_data_string[ADDRESS_SIZE+1];
    EncodeLong(new_data, new_data_string);
    new_node->col_locations  = (char *)calloc(ADDRESS_SIZE+1, sizeof(char));
    // bzero(new_node->col_locations,ADDRESS_SIZE);
    printf("New data string %ld encoding",DecodeLong(new_data_string));
    //strcpy(new_node->col_locations, NULL);

    strcpy(new_node->col_locations, new_data_string);
    printf("col_locations %ld end ",DecodeLong(new_node->col_locations));
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
int main()
{
    int index_present = 0; 
    int num_map_cols = 0;
    struct MapList *head = NULL;
    FILE* fp;
    fp = fopen("trainsmall.csv", "r");
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

    int get_colno = 2;
    

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
            // collist = (char *)calloc(read * sizeof(char));
            char *tmp;
            
            collist = strdup(row); 
            num_cols = count_cols(row);
            // printf("Number cols: %d\nCharacters read: %ld\nCol list: %s\n",num_cols, read, collist);
            //find col name for index no in csv header. Note index no starts from 1
            int index_no = 1;
            tmp = strdup(collist);
            strcpy(row, collist);
            col_from_index = getfield(tmp, index_no);

            printf("col %s at index %d\n", collist, get_colno);
            num_map_cols++;

        }
        num_rows++;
        cumulative_length = prev_len + cumulative_length;
        // vector_add(&row_locations, cumulative_length);
        printf(" cum %d ",cumulative_length);
        append(&head, cumulative_length);
        printList(head,10);
        prev_len = read;     
 
    }

    //Creating index on column requested by user get_colno (here set to 2 manually)
    fseek(fp, 0, SEEK_SET);
    num_rows = 0;
    cumulative_length = 0, prev_len = 0;
    char *tmp = strdup(collist);
    char *column_name = getfield(tmp,get_colno);
    //Realloc to update col_from_index which contains column names present in index
    col_from_index = (char *) realloc(col_from_index,strlen(col_from_index)+strlen(column_name)+1);
    strcat(col_from_index,",");
    strcat(col_from_index,column_name);
    
    //Scanning file from beginning
    while ((read = getline(&row, &len, fp)) != -1)
    {
        // collist_map: stores whatever columns are actually there in the map right now
        // Adaptive index builder, when new query comes checks if column is present in index
        // if yes, search file based on index
        // if not, augment the index ---> 
        // get row based on 1st column index / vector row_loc : fseek maybe
        // tok tok karke 4th column ka address maybe?
        // add to maplist
        
      
        cumulative_length = prev_len + cumulative_length;

        char *tmp = strdup(row);
        struct MapList *last; 
        if(num_rows ==0)
        {
            last = head;
            num_map_cols++;
        }
        printf("Before: %s %ld %d ",last->col_locations,DecodeLong(last->col_locations),strlen(last->col_locations));
        //Realloc col_locations to store column address of new column
        
        last->col_locations  = (char *)realloc(last->col_locations, (num_map_cols) * ADDRESS_SIZE * sizeof(char) + 1);
        char new_data_string[ADDRESS_SIZE];
        bzero(last->col_locations+8,ADDRESS_SIZE);
        long newlen = cumulative_length+getlength(tmp,get_colno);
        printf("NEW LENGTH %ld ",newlen);
        int byteEncoded = EncodeLong(newlen, new_data_string);
        strcpy(last->col_locations+8,new_data_string);
        printf("After: %ld  %ld \n",DecodeLong(last->col_locations),DecodeLong(last->col_locations+ADDRESS_SIZE));

        last = last->next;
        //append(&head, cumulative_length + getlength(tmp, get_colno));
        
        
        prev_len = read;    
        num_rows++;
 
    }
    printf("num_map_cols = %d\n", num_map_cols);

    printf("\nColumns:\n%s\n", collist);
    printf("Number of columns: %d\n\n",num_cols);
    printf("Index list built so far on following columns: %s\n\n",col_from_index);
    
    printf("Row locations based on index build so far:\n");
    printList(head, 10); // we want start address of first 10 rows
    //set pointer of file to desired position and read rows
    fseek(fp, 0, SEEK_SET);
    
    fclose(fp);
}