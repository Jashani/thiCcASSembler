#include "errors.h"

void print_error(int current_line) {
    printf("Line %d: ", current_line);
    switch (g_error) {
        case ERROR_SYNTAX:
            printf("Syntax error.\n");
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
    }
}