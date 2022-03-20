#include "globals.h"
#include "common.h"

#ifndef ERRORS_H
#define ERRORS_H

typedef enum error_types {
    NO_ERRORS = 0,
    ERROR_SYNTAX,
    ERROR_LINE_TOO_LONG,
    ERROR_FAILED_READ,
    ERROR_LABEL_TOO_LONG,
    ERROR_BAD_LABEL,
    ERROR_RESERVED_KEYWORD,
    ERROR_BAD_DIRECTIVE,
    ERROR_BAD_INSTRUCTION,
    ERROR_MISSING_ARGUMENTS,
    ERROR_SYMBOL_EXISTS,
    ERROR_MISSING_QUOTES,
    ERROR_EMPTY_STRING,
    ERROR_BAD_ARGUMENT
} error_type;

void print_error(int current_line);

#endif