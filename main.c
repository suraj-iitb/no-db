#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "vector.h"

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

const char* getfield(char* row, int col_num)
{
    const char* tok;
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--col_num)
            return tok;
    }
    return NULL;
}

char *collist, *collist_map;
typedef struct MapList {
    char *col_locations;
    struct MapList *next;
} MapList;



void append(struct MapList** head_ref, int new_data) 
{ 
    /* 1. allocate node */
    struct MapList* new_node = (struct MapList*) malloc(sizeof(struct MapList)); 
  
    struct MapList *last = *head_ref;  /* used in step 5*/
  
    /* 2. put in the data  */
    char new_data_string[20];
    //itoa(new_data, new_data_string, 10);
    sprintf(new_data_string,"%d", new_data);
    new_node->col_locations  = (char *)malloc(20 * sizeof(char));
    int num_zeros = 20-strlen(new_data_string);
    memset(new_node->col_locations, '0', num_zeros*sizeof(char)); 
    strcpy(new_node->col_locations+num_zeros,new_data_string);
    printf("SIZEOF %d lenght %d",sizeof(new_node->col_locations),strlen(new_node->col_locations));
    /* 3. This new node is going to be the last node, so make next of 
          it as NULL*/
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
    int num_rows = 0, num_cols = 0,cumulative_length = 0, prev_len = 0;
    //int *row_loc;
    //row_loc[0] = 0;
    //printf("HRLOO\n");
    vector row_locations;
    vector_init(&row_locations);
    if (fp == NULL)
        exit(EXIT_FAILURE);

    struct MapList* head = NULL; 

    while ((read = getline(&row, &len, fp)) != -1)
    {
        if(num_rows == 0)
        {
            collist = (char *)malloc(read * sizeof(char));
            printf("Numbercols %d, read: %d row %s",num_cols, read, row);
            strcpy(collist,row);
            num_cols = count_cols(row);

        }
        char* tmp = strdup(row);
        //printf("Field 3 would be %s ", getfield(tmp, 3));
        // NOTE strtok clobbers tmp
        free(tmp);
        //printf("Row number: %d, File pointer: %ld ",num_rows,*fp);
        num_rows++;
        //row_loc[num_rows] = len + row_loc[num_rows-1];
        cumulative_length = prev_len + cumulative_length;
        //printf("prev_len %d Row loc: %d \n",prev_len,cumulative_length);
        vector_add(&row_locations, cumulative_length);
        append(&head,cumulative_length);
        prev_len = read;

        //len = 0;
    }
    printf("Number of fields: %d\n",num_cols);
    fseek(fp,0,SEEK_SET);
    read = getline(&row, &len, fp);
    printf("Row: %s \n",row);
    printf("Row locations: \n");
    int i = 0;
    /*for (i = 0; i < vector_count(&row_locations); i++) {
       // printf("%d ", vector_get(&row_locations, i));
    }*/
    /*int i = 0;
    for(i=0 ; i<num_rows; i++)
        printf("%d ",row_loc[i]);
    */
    printf("\nLength: %d ",strlen("326835,0,92354,18,626,86.3866666666667,55,102.019618156945,0,431,37.8986666666667,9,118,11,41,102,-30,53,67,20,0,24,saturday,tuesday,1"));
    printf("Collist %s\n",collist);
    printList(head);
    fclose(fp);
}
