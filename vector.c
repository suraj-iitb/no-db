#include "vector.h"

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

int vector_getloc(vector *v, void *e)
{
    for(int i = 0; i < v->count; i++)
    {
        if(strcmp((char *)e, v->data[i]) == 0)
            return i;
    }

    return -1;
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
    free(v);
}