#include "linear_index.h"

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
