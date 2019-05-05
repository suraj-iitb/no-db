#include "wrapper.h"
#include "table.h"
#include "linear_index.h"

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
  InputBuffer* input_buffer = create_input_buffer();
  while (true) {
    printf("No-Db > ");
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else if (strncmp(input_buffer->buffer, "create table", 12) == 0) {
        char *token;
        /* get the first token */
        token = strtok(input_buffer->buffer, " ");
        /* walk through other tokens */
        while( token != NULL ) {
          file = strdup(token);
          token = strtok(NULL, " ");
        }
        create_table(file);
    }
    else if (strncmp(input_buffer->buffer, "select", 6) == 0)  {
      // printf("implement for retrieval of '%s'.\n", input_buffer->buffer);
      create_initial_index(file);
      create_index(2, file); // 2 is column no.
      
    }
  }
}