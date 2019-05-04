#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct MapList {
    char *col_locations;
    struct MapList *next;
} MapList;

#define ADDRESS_SIZE sizeof(long long)

void printList(struct MapList *node, long long len);
void append(struct MapList** head_ref, long long new_data);