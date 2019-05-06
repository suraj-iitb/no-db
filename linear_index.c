#include "linear_index.h"
#include "table.h"
#include "col_row_op.h"

#include <stdarg.h>
#include "vector.h"

long long num_map_cols = 0;


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

void printList(struct MapList *node, long long len, int cols) 
{ 
	int i = 0;	
	while (node != NULL && len != 0) 
	{ 
		for(i = 0; i < cols; i++)
		{
			printf(" %ld \t", DecodeLong(node->col_locations+i*(ADDRESS_SIZE))); 
  		// printf(" --------- \n");
  	// }
		}
		printf("\n --------- \n");
		node = node->next; 
		len--;
	} 
	printf("\n\n");
} 

void printResults(char *file, vector *query_col_indices)
{
	FILE *fp = fopen(file, "r");
	int i, j, k;
	char *row = NULL;
	size_t len = 0;
	ssize_t read;
	struct MapList *last; 
	last = head;
	printf("In printResults\n");

	for(j = 0; j < 10; j++, last = last->next)
	{
		for (i = 0; i < vector_count(query_col_indices); i++)
		{
   			// col_from_index_posmap[get_colno] = num_map_cols;
   			// printf("\nHello1\n");
			k = (int)(query_col_indices->data[i]);
			fseek(fp, DecodeLong(last->col_locations+(col_from_index_posmap[k]*ADDRESS_SIZE)), SEEK_SET);
	   		// printf("Hello4\n");
			read = getline(&row, &len, fp);
			printf("%s \t", getfield(row, 1));
      		// sum += va_arg(valist, int);
		}
		printf("\n");
	}
	fclose(fp);  
}


int fetchAll(char *file)
{
	if(head == NULL)
	{
		create_initial_index(file);
	}
	FILE *fp = fopen(file, "r");
	int j;
	char *row = NULL;
	size_t len = 0; ssize_t read;
	struct MapList *last = head; 
	for(j = 0; j < 10; j++, last = last->next)
	{
		fseek(fp, DecodeLong(last->col_locations), SEEK_SET);
		read = getline(&row, &len, fp);
		printf("%s \t", row);
		printf("\n");
	}
	fclose(fp);
}

int fetchSome(char *file, vector *query_col_indices)
{
	// printf("vector_count(query_col_indices): %d\n", vector_count(query_col_indices));
	int qn = vector_count(query_col_indices);
	// printf("vector_count(query_col_indices): %d\n", qn);
	printf("In fetchSome\n");
	if(head == NULL)
	{
		create_initial_index(file);
	}
	int k;
	update_index(file, query_col_indices);
	// for(int i = 0; i < qn; i++)
	// {
	// 	k = (int)(query_col_indices->data[i]); //1...n
	// 	if(col_from_index_posmap[k] < 0)
	// 	{
	// 		// printf("In fetchSome3 k = %d\n", k);
	// 		create_index(k, file);
	// 		// printf("Index made Done!\n");
	// 	}

	// }
	printResults(file, query_col_indices); 
}

//Creating index on column requested by user. get_colno contains column no of column
void create_index(int get_colno, char *file){

	FILE *fp = fopen(file, "r");
	printf("In create_index\n");
	fseek(fp, 0, SEEK_SET);
	long long num_rows = 0;
	long long cumulative_length = 0, prev_len = 0;
	ssize_t read;
	char *tmp;
	struct MapList *last; 

	char *row = NULL;
	size_t len;

	if(allColNames_str != NULL){

		tmp = strdup(allColNames_str);

		char *column_name = getfield(tmp, get_colno);
		printf("%s\n", column_name);

    //Realloc to update col_from_index which contains column names for all indices
		col_from_index = (char *) realloc(col_from_index, strlen(col_from_index)+strlen(column_name)+1);

		if (num_rows != 0)
			strcat(col_from_index,",");
		strcat(col_from_index,column_name);

    //Scanning file from beginning
		while ((read = getline(&row, &len, fp)) != -1)
		{
			cumulative_length = prev_len + cumulative_length;
			char *tmp = strdup(row);

			if(num_rows == 0)
			{
				last = head;
            // printf("num_map_cols: %d\n", num_map_cols);
				col_from_index_posmap[get_colno] = num_map_cols;
				num_map_cols++;
            // SetBit(col_from_index_bitmap, get_colno);

			}
			last->col_locations = (char *)realloc(last->col_locations, (num_map_cols) * ADDRESS_SIZE * sizeof(char) + 1);
			char new_data_string[ADDRESS_SIZE];
			bzero(last->col_locations+(num_map_cols-1)*ADDRESS_SIZE,ADDRESS_SIZE);
			long newlen = cumulative_length+getlength(tmp,get_colno-1);
        // printf("NEW LENGTH %ld ",newlen);
			int byteEncoded = EncodeLong(newlen, new_data_string);
			strcpy(last->col_locations+(num_map_cols-1)*ADDRESS_SIZE,new_data_string);
        // printf("After: %ld  %ld \n",DecodeLong(last->col_locations),DecodeLong(last->col_locations+ADDRESS_SIZE));
			last = last->next;
        //append(&head, cumulative_length + getlength(tmp, get_colno));
			prev_len = read;    
			num_rows++;

		}
	}
	printf("Numcols: %d; Row locations based on index build so far:\n",num_map_cols);
	printList(head, 10, num_map_cols); 
	fclose(fp);
  // printf("Done!\n");
}

//create index on first column
void create_initial_index(char * file)
{
	printf("In create_initial_index\n");
	FILE* fp = fopen(file, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	char *row = NULL;
	size_t len = 0;
	ssize_t read;
	int num_cols = 0;
	long num_rows = 0;
	long long cumulative_length = 0, prev_len = 0;
	struct MapList *last = head;  

	while ((read = getline(&row, &len, fp)) != -1)
	{

		if(num_rows == 0)
		{
            //reading columns i.e header of csv file
			allColNames_str = strdup(row); 
			num_cols = count_cols(row);

            //find col name for index no in csv header. Note index no starts from 1
			char *tmp = strdup(allColNames_str);
			strcpy(row, allColNames_str);
			col_from_index = getfield(tmp, 1);
            // SetBit(col_from_index_bitmap, 1);

            // printf("col %s at index %d\n", col_from_index, 1);
			col_from_index_posmap[1] = num_map_cols;
			num_map_cols++;
			

		}
		num_rows++;

		cumulative_length = prev_len + cumulative_length;
        // vector_add(&row_locations, cumulative_length);
		append(&head, cumulative_length);

		prev_len = read;      
	}

    // printf("\nColumns:\n%s\n", allColNames_str);
    // printf("Number of columns: %d\n\n",num_cols);
	printf("Index list built so far on following columns: %s\n\n", col_from_index);
	last = head;
	row = NULL;
	for(int i = 0; i < 10; i++, last = last->next)
	{
		fseek(fp, DecodeLong(last->col_locations), SEEK_SET);
		read = getline(&row, &len, fp);
		printf("-- %s \n", getfield(row, 1));
	}
	printf("Row locations based on index build so far:\n");
    printList(head, 10, num_map_cols); // we want start address of first 10 rows
    
    fclose(fp);
}

void update_index(char *file, vector *query_col_indices)
{

	FILE *fp = fopen(file, "r");
	printf("In update_index\n");
	int qn = vector_count(query_col_indices);
	// printf("vector_count(query_col_indices): %d",);
	long long num_rows = 0;
	long long cumulative_length = 0, prev_len = 0;
	ssize_t read;
	char *tmp;
	struct MapList *last; 
	int get_colno;
	char *row = NULL;
	size_t len;
	long newlen;
	char malloc_required = '0';
	if(head == NULL)
	{ 
		malloc_required = '1';
	}
	int prev_num_map_cols = num_map_cols;
	int cols_to_construct[qn];
	int qni = 0;
	while ((read = getline(&row, &len, fp)) != -1)
	{
		cumulative_length = prev_len + cumulative_length;
		char *tmp = strdup(row);
		int k = 0;
		if(num_rows == 0)
		{
			last = head;
			for(int i = 0; i < qn; i++)
			{
				get_colno = query_col_indices->data[i];
				if(col_from_index_posmap[get_colno] == -1)
				{
					col_from_index_posmap[get_colno] = num_map_cols++;
					cols_to_construct[qni++] = get_colno;
				}
			}
		}
		last->col_locations = (char *)realloc(last->col_locations, (num_map_cols) * ADDRESS_SIZE * sizeof(char) + 1);
		for(int i = 0; i < qni; i++)
		{
			*tmp = strdup(row);
			get_colno = cols_to_construct[i];
				
			char new_data_string[ADDRESS_SIZE];
			bzero(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, ADDRESS_SIZE);
			newlen = cumulative_length + getlength(tmp,get_colno-1);
			int bytesEncoded = EncodeLong(newlen, new_data_string);
			strcpy(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, new_data_string);
			free(tmp);
			// printf("After: %ld  %ld \n",DecodeLong(last->col_locations),DecodeLong(last->col_locations+ADDRESS_SIZE));
				
		}
		last = last->next;
		prev_len = read;    
		num_rows++;

	}
	fclose(fp);
	printf("Row locations based on index build so far:\n");
    printList(head, 10, num_map_cols); // we want start address of first 10 rows
  // printf("Done!\n");
}


// void printResults_deprecated(char *file, int num_cols_to_print, ...)
// {
//    FILE *fp = fopen(file, "r");
//    va_list valist;
//    int i, j;
//    /* initialize valist for num number of arguments */
//    /* access all the arguments assigned to valist */
//    char *row = NULL;
//     size_t len = 0;
//     ssize_t read;
//     struct MapList *last; 
//    va_start(valist, num_cols_to_print);
//    last = head;
//    printf("In printResults\n");

//    for(j = 0; j < 10; j++, last = last->next)
//    {
//    		for (i = 0; i < num_cols_to_print; i++)
//    		{
//    			// col_from_index_posmap[get_colno] = num_map_cols;
//    			// printf("i: %d, pos: %d\n", i, col_from_index_posmap[i]);
// 	   		// fseek(fp, DecodeLong(last->col_locations+(col_from_index_posmap[i]*ADDRESS_SIZE)), SEEK_SET);
// 	   		// read = getline(&row, &len, fp);
// 	    	// printf("%s \t", getfield(row, 1));
//       		// sum += va_arg(valist, int);

//    		}
//    		printf("\n");
//    }
//    /* clean memory reserved for valist */
//    va_end(valist);
//    fclose(fp);  
// }
