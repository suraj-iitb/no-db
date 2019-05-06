#include "wrapper.h"
#include "table.h"
#include "linear_index.h"
#include "vector.h"

InputBuffer* create_input_buffer() {
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
  if (bytes_read <= 0) {
    printf("Please provide command to execute!\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline which terminates getline
  input_buffer->input_length = bytes_read - 1;
  //index from "0" to  "(input_buffer->input_length - 1)"
  //put NULL character to terminate the input string which is at index "input_buffer->input_length"
  input_buffer->buffer[input_buffer->input_length] = '\0';
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void cli_for_db() {
  char *file;
  char *token;
  InputBuffer* input_buffer = create_input_buffer();
  vector query_col_indices;
  while (true) {
    printf("No-Db > ");
    read_input(input_buffer);

    if (strcasecmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else if (strncasecmp(input_buffer->buffer, "create table", 12) == 0) {
        
        /* get the first token */
        token = strtok(input_buffer->buffer, " ");
        /* walk through other tokens */
        while( token != NULL )
        {
            file = strdup(token);
            token = strtok(NULL, " ");
        }
        create_table(file);
    }
    else if (strncasecmp(input_buffer->buffer, "select", 6) == 0)  {
      // printf("implement for retrieval of '%s'.\n", input_buffer->buffer);
      /*
      select * from
      select col1, col2.. from
      select * from where
      select col1, col2... from where
      */
      
      vector_init(&query_col_indices);
      char someCase = '1';
      //case 1
      token = strtok(input_buffer->buffer, " ");   
      /* walk through other tokens */
      // int colsToFetch_bitmap[(num_cols+1)/32 + 1];
      // for(int i = 0; i < (num_cols+1)/32 + 1; i++)
      // {
      //   colsToFetch_bitmap[i] = 0;
      // }
      while(strcasecmp(token, "from" ) != 0)
      {
        // printf("--%s--\n", token);
        if(strcmp(token, "*") == 0)
        {
          someCase = '0';
          file = strtok(NULL, ", \n"); //from
          file = strtok(NULL, ", \n"); //filename
          fetchAll(file);
          break;
        }
        // printf("Location of %s %d\n", token, vector_getloc(&allColNames_list, token));
        token = strtok(NULL, ", \n");
        // colsToFetch_bitmap[vector_getloc(&allColNames_list, token)] = 1;
        //it's reverse, in each int
        int k = vector_getloc(&allColNames_list, token);
        // printf("Location of %d\n", k);
        if(k > 0)
        {
          vector_add(&query_col_indices, k);
          // SetBit(colsToFetch_bitmap, k);
        }
        // printf("colsToFetch_bitmap[0]: %d\n", colsToFetch_bitmap[0]);
      }
      if(someCase == '1')
      {
        file = strtok(NULL, ", \n");

        fetchSome(file, &query_col_indices);
      }
    }
  }
}