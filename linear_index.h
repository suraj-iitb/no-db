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

#define ADDRESS_SIZE sizeof(long long)

void printList(struct MapList *node, long long len);
void append(struct MapList** head_ref, long long new_data);
#endif