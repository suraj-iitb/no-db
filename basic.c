#include "basic.h"
void fetchWithoutIndex(char * file, vector *query_col_indices)
{
	FILE* fp = fopen(file, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	char *row = NULL;
	size_t len = 0;
	ssize_t read;
	char * value;

	int qn = vector_count(query_col_indices);
	int get_colno;
	int num_rows = 0;
	while ((read = getline(&row, &len, fp)) != -1)
	{

        for(int i = 0; i < qn; i++)
		{
			int count_of_cols = 0;
			get_colno = query_col_indices->data[i];
			char *temp =  strdup(row);
			value = getfield(temp, get_colno);
			printf("%s\t", value);

		}   
		printf("\n");  
		num_rows++;
		if(num_rows > 100)
			break;
	}
    
    fclose(fp);
}