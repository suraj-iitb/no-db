#include "linear_index.h"
#include "table.h"
#include "col_row_op.h"
#include "vector.h"

long long num_map_cols = 0;


/*Add col_locations to MapList*/
struct MapList *append(struct MapList** head_ref, long long new_data) 
{ 
    /* 1. allocate node */
	struct MapList *new_node = (struct MapList*) calloc(sizeof(struct MapList), 1); 
    struct MapList *last = *head_ref;  /* used in step 5*/

    /* 2. put in the data  */
	char new_data_string[ADDRESS_SIZE];

	EncodeLong(new_data, new_data_string);
	new_node->col_locations  = (char *)calloc(ADDRESS_SIZE , sizeof(char));
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
	return new_node; 
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

	for(j = 0; j < 101; j++, last = last->next)
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
	printf("In fetchSome\n");
	if(head == NULL)
	{
		create_initial_index(file);
	}
	int k;
	aupdate_index(file, query_col_indices);
	printResults(file, query_col_indices); 
}

//Creating index on column requested by user. get_colno contains column no of column


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
            // printf("col %s at index %d\n", col_from_index, 1);
			col_from_index_posmap[1] = num_map_cols;
			num_map_cols++;
		}
		num_rows++;

		cumulative_length = prev_len + cumulative_length;
		append(&head, cumulative_length);

		prev_len = read;      
	}

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
	struct MapList *last = head; 
	int get_colno;
	char *row = NULL;
	size_t len;
	long newlen;
	// char malloc_required = '0';
	// if(NULL == head)
	// { 
	// 	malloc_required = '1';
	// }
	int prev_num_map_cols = num_map_cols;
	int cols_to_construct[qn];
	int qni = 0;
	while ((read = getline(&row, &len, fp)) != -1)
	{

		cumulative_length = prev_len + cumulative_length;
		// if(malloc_required == '1')
		// {
		// 	last = append(&head, cumulative_length);
		// 	// last = last->next;
		// }
		// printf("Hello1\n");
		char *tmp = strdup(row);
		int k = 0;
		if(num_rows == 0)
		{
			// if(malloc_required == '1')
			// {
			// 	col_from_index_posmap[1] = num_map_cols;
			// 	num_map_cols++;
			// }
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
		// printf("Hello2\n");
		last->col_locations = (char *)realloc(last->col_locations, (num_map_cols) * ADDRESS_SIZE * sizeof(char) + 1);
		// printf("Hello3 %d: num_map_cols\n", num_map_cols);
		for(int i = 0; i < qni; i++)
		{
			*tmp = strdup(row);
			get_colno = cols_to_construct[i];

			char new_data_string[ADDRESS_SIZE];
			bzero(new_data_string, ADDRESS_SIZE);
			bzero(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, ADDRESS_SIZE);
			newlen = cumulative_length + getlength(tmp,get_colno-1);
			int bytesEncoded = EncodeLong(newlen, new_data_string);
			strcpy(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, new_data_string);
			free(tmp);
			// printf("After: %ld  %ld \n",DecodeLong(last->col_locations),DecodeLong(last->col_locations+ADDRESS_SIZE));
			// printf("Hello4\n");	
		}
		// if(malloc_required == '0')
		// {
		// 	last = last->next;
		// }
		prev_len = read;    
		num_rows++;

	}
	fclose(fp);
	printf("Row locations based on index build so far:\n");
    printList(head, 10, num_map_cols); // we want start address of first 10 rows
  // printf("Done!\n");
}

void aupdate_index(char *file, vector *query_col_indices)
{

	FILE *fp = fopen(file, "r");
	FILE *fp2 = fopen(file, "r");
	printf("In aupdate_index\n");
	int qn = vector_count(query_col_indices); //number of columns queried
	long long num_rows = 0;
	long long cumulative_length = 0, prev_len = 0; long long newlen; //calculating offset for each column
	
	char *row = NULL; char *row2 = NULL; char *tmp;
	struct MapList *last = head; 
	ssize_t read; size_t len, len2; //reading addresses

	int prev_num_map_cols = num_map_cols;
	int cols_to_construct[qn]; //the ones that will need indices
	int brother_cols[qn]; //the ones closest to the ones above
	int qni = 0; //number of cols for which index will be constructed in this function call
	int get_colno;
	int min_bro = 1600;
	// printf("Hello1\n");
	for(int i = 0; i < qn; i++)
	{
		get_colno = query_col_indices->data[i];
		if(col_from_index_posmap[get_colno] == -1) //does index exist? no?
		{
			for(int i = get_colno-1; i >= 1; i--) //find the closest brother, going backwards
			{
				if(col_from_index_posmap[i] >= 0) //if index exists, you are my brotheeerrrr
				{
					brother_cols[get_colno] = i; 
					if(min_bro>brother_cols[get_colno])
						min_bro=brother_cols[get_colno];
					printf("brother_cols: %d",brother_cols[get_colno]);
					break;
				}
			}
			col_from_index_posmap[get_colno] = num_map_cols++; //new column's offset in the index
			cols_to_construct[qni] = get_colno; //store the column no.s that we will be creating index on
			qni++;
		}
	}
	// printf("Hello2\n");
	if(qni > 0)
	{
		int te=0;
		printf("min_bro = %d\n",min_bro);
		while (last != NULL)
		{

			cumulative_length = prev_len + cumulative_length;
			
		// printf("Hello1\n");
			int k = 0;
		// printf("Hello2\n");
			fseek(fp2, DecodeLong(last->col_locations+(col_from_index_posmap[min_bro])*ADDRESS_SIZE), SEEK_SET);
			read = getline(&row2, &len2, fp2);
			char *tmp = strdup(row2);
			last->col_locations = (char *)realloc(last->col_locations, (num_map_cols) * ADDRESS_SIZE * sizeof(char) + 1);
		// printf("Hello3 \n");
			for(int i = 0; i < qni; i++)
			{
				if(num_rows == 0)
				{
					printf("%d\n", cols_to_construct[i]);
				}
				tmp = strdup(row2);
				get_colno = cols_to_construct[i]; 
				char new_data_string[ADDRESS_SIZE];
				bzero(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, ADDRESS_SIZE);

				if(num_rows < 10)
				{
					// printf("After: %ld \n",DecodeLong(last->col_locations+(col_from_index_posmap[brother_cols[get_colno]])));
				}
				newlen = DecodeLong(last->col_locations+(col_from_index_posmap[min_bro]*ADDRESS_SIZE))
				+ getlength(tmp, get_colno - min_bro);
			// printf("newlen: %ld\n", newlen);
				int bytesEncoded = EncodeLong(newlen, new_data_string);
				strcpy(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE, new_data_string);
			// free(tmp);
				// if (te<10)
				// printf("te is %ld\n", DecodeLong(last->col_locations+(prev_num_map_cols+i)*ADDRESS_SIZE));
				te++;
			// printf("Hello4\n");	
			}
			last = last->next;
			prev_len = read;    
			num_rows++;

		}
	}
	
	fclose(fp);
	printf("Row locations based on index build so far:\n");
    printList(head, 10, num_map_cols); // we want start address of first 10 rows
  // printf("Done!\n");
}
