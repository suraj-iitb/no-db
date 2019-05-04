#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InputBuffer_t {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
};
typedef struct InputBuffer_t InputBuffer;

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

int main() {
  InputBuffer* input_buffer = create_input_buffer();
  while (true) {
    printf("No-Db > ");
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("implement for retrieval of '%s'.\n", input_buffer->buffer);
      \\ call xyz method to retieve rows
    }
  }
}