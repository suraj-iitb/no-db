#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "vector.h"

#define ADDRESS_SIZE sizeof(long long)
typedef char byte;

typedef union {
    long long ll;
    byte   bytes[8];
} LongBytes;

/*
 Naive encoding of a 64-bit long to 8 bytes. 
 */
int
EncodeLong(long long l, byte *bytes) {
    LongBytes lb;
    lb.ll = l;
    memcpy(bytes, lb.bytes, 8);
    return 8;
}

/*
  Translates 8 bytes pointed to by 'bytes' to a 64-bit long.
 */
long long
DecodeLong(byte *bytes) {
    LongBytes lb;
    memcpy(lb.bytes, bytes, 8);
    return lb.ll;
}

char *collist, *collist_map;
typedef struct MapList {
    char *col_locations;
    struct MapList *next;
} MapList;

struct MapList* head = NULL; 

void vector_init(vector *v)
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}

int vector_count(vector *v)
{
    return v->count;
}

void vector_add(vector *v, void *e)
{
    if (v->size == 0) {
        v->size = 10;
        v->data = malloc(sizeof(void*) * v->size);
        memset(v->data, '\0', sizeof(void) * v->size);
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

void vector_set(vector *v, int index, void *e)
{
    if (index >= v->count) {
        return;
    }

    v->data[index] = e;
}

void *vector_get(vector *v, int index)
{
    if (index >= v->count) {
        return;
    }

    return v->data[index];
}

void vector_delete(vector *v, int index)
{
    if (index >= v->count) {
        return;
    }

    v->data[index] = NULL;

    int i, j;
    void **newarr = (void**)malloc(sizeof(void*) * v->count * 2);
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

/*
int main()
{
    FILE *fin;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //Open .csv file
    fin = fopen("datafile.csv","r");
    printf("fin: %p",fin);

    if (fin == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fin)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s pointer: %p \n", line,fin);
    }

    fclose(fin);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}
*/

const int count_cols(char* row)
{
    const char* tok;
    int num_cols = 0;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        num_cols++;
    }
    return num_cols;
}

const char* getfield(char* row, int col_num) //where col_num starts from 1
{
    const char* tok;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--col_num)
            return tok;
    }
    return NULL;
}

void append(struct MapList** head_ref, long long new_data) 
{ 
    /* 1. allocate node */
    struct MapList* new_node = (struct MapList*) malloc(sizeof(struct MapList)); 
    struct MapList *last = *head_ref;  /* used in step 5*/
  
    /* 2. put in the data  */
    char new_data_string[ADDRESS_SIZE];
    //itoa(new_data, new_data_string, 10);

    EncodeLong(new_data, new_data_string);
    new_node->col_locations  = (char *)malloc(ADDRESS_SIZE * sizeof(char));
    strcpy(new_node->col_locations, new_data_string);

    // sprintf(new_data_string,"%ld", new_data);
    // new_node->col_locations  = (char *)malloc(ADDRESS_SIZE * sizeof(char));
    // int num_zeros = 20-strlen(new_data_string);
    // memset(new_node->col_locations, '0', num_zeros*sizeof(char)); 
    // strcpy(new_node->col_locations+num_zeros,new_data_string);

    // printf("SIZEOF %d lenght %d",sizeof(new_node->col_locations),strlen(new_node->col_locations));

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

void printList(struct MapList *node) 
{ 
  while (node != NULL) 
  { 
     printf(" %s ", node->col_locations); 
     node = node->next; 
  } 
} 



int main()
{
    FILE* fp;
    fp = fopen("train.csv", "r");
    
    char *row = NULL;
    size_t len = 0;
    ssize_t read;
    int num_cols = 0;
    long num_rows = 0;
    long long cumulative_length = 0, prev_len = 0;
    //int *row_loc;
    //row_loc[0] = 0;
    // printf("HRLOO\n");
    vector row_locations;
    vector_init(&row_locations);
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&row, &len, fp)) != -1)
    {
        if(num_rows == 0)
        {
            collist = (char *)malloc(read * sizeof(char));
            strcpy(collist,row);
            num_cols = count_cols(row);
            printf("Numbercols %d, read: %ld row %s",num_cols, read, row);
            
            char *tmp = strdup(row);
            char *colToIndex = getfield(tmp,1);
            
            collist_map = (char *)malloc(strlen(colToIndex) * sizeof(char));
            strcpy(collist_map,colToIndex);

            free(tmp);
            tmp = strdup(row);
            free(colToIndex);
            colToIndex = getfield(tmp,4);

            collist_map = (int *)realloc(collist_map, (strlen(collist_map)+strlen(colToIndex)) * sizeof(char));
            strcpy(collist_map,colToIndex); 

        }
        num_rows++; char *firstcol = getfield(tmp,1);
        //row_loc[num_rows] = len + row_loc[num_rows-1];
        cumulative_length = prev_len + cumulative_length;
        //printf("prev_len %d Row loc: %d \n",prev_len,cumulative_length);
        vector_add(&row_locations, cumulative_length);
        append(&head, cumulative_length);
        prev_len = read;

        //len = 0;
    }
    printf("Number of fields: %d\n",num_cols);
    fseek(fp,0,SEEK_SET);
    read = getline(&row, &len, fp);
    printf("Row: %s \n",row);
    printf("Row locations: \n");
    int i = 0;
    printf("collist_map %s\n",collist_map);
    // printList(head);
    struct MapList* node = head; 
    while (i < 10) 
  { 
     printf(" %ld ", DecodeLong(node->col_locations)); 
     node = node->next; 
     i++;
  } 
    fclose(fp);
}
