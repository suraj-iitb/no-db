#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InputBuffer_t {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
}InputBuffer;

InputBuffer* create_input_buffer();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
void cli_for_db();