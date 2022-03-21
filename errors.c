#include "errors.h"

void print_error(int current_line) {
    printf("Error on line %d: ", current_line);
    switch (g_error) {
        case ERROR_SYNTAX:
            printf("Syntax error.\n");
            break;
        case ERROR_LINE_TOO_LONG:
            printf("Line too long (Max %d characters).\n", MAX_LINE_LENGTH);
            break;
        case ERROR_FAILED_READ:
            printf("Failed to read line.\n");
            break;
        case ERROR_LABEL_TOO_LONG:
            printf("Label too long (Max %d characters).\n", MAX_LABEL_LENGTH);
            break;
        case ERROR_BAD_LABEL:
            printf("Bad label format.\n");
            break;
        case ERROR_RESERVED_KEYWORD:
            printf("Usage of reserved keywords for other purposes is not allowed.\n");
            break;
        case ERROR_BAD_DIRECTIVE:
            printf("Unrecognised directive.\n");
            break;
        case ERROR_MISSING_ARGUMENTS:
            printf("Arguments expcted but none given.\n");
            break;
        case ERROR_BAD_INSTRUCTION:
            printf("Unrecognised instruction.\n");
            break;
        case ERROR_SYMBOL_EXISTS:
            printf("Symbol already exists.\n");
            break;
        case ERROR_MISSING_QUOTES:
            printf("String directive must use double quotes (\").\n");
            break;
        case ERROR_EMPTY_STRING:
            printf("String cannot be empty.\n");
            break;
        case ERROR_BAD_ARGUMENT:
            printf("Bad argument.\n");
            break;
        case ERROR_CANNOT_LABEL_DIRECTIVE:
            printf("May only label '.data' and '.string' directives.\n");
            break;
        case ERROR_ADDRESSING:
            printf("Bad addressing, ensure legal operand usage.\n");
            break;
        case ERROR_ARGUMENT_STRUCTURE:
            printf("Bad argument structure for instruction.\n");
            break;
    }
}