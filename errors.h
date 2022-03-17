#include "globals.h"
#include "common.h"

typedef enum error_types {
    NO_ERRORS = 0,
    ERROR_SYNTAX,
    ERROR_LINE_TOO_LONG,
    ERROR_FAILED_READ,
    ERROR_LABEL_TOO_LONG,
    ERROR_BAD_LABEL,
    ERROR_RESERVED_KEYWORD,
    ERROR_BAD_DIRECTIVE,
    ERROR_MISSING_ARGUMENTS,
} error_type;

void print_error(int current_line);