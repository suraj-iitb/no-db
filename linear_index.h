#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#ifndef INDEX_H__
#define INDEX_H__

typedef struct MapList {
    char *col_locations;
    struct MapList *next;
} MapList;

struct MapList *head;
char *col_from_index;
int *col_from_index_bitmap;
int *col_from_index_posmap;

#define ADDRESS_SIZE sizeof(long long)

void printList(struct MapList *node, long long len, int num_cols);
void append(struct MapList** head_ref, long long new_data);
#endif