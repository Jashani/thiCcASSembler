#include "errors.h"

void print_error(int current_line) {
    printf("Line %d: ", current_line);
    switch (g_error) {
        case ERROR_LINE_TOO_LONG:
            printf("Line too long (Max %d characters)", MAX_LINE_LENGTH);
    }
}