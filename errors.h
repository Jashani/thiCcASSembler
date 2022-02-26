#include "globals.h"
#include "common.h"

typedef enum error_types {
    ERROR_LINE_TOO_LONG = 2
} error_type;

void print_error(int current_line);