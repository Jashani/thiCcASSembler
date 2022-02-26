#include "first_pass.h"

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;

    g_instruction_counter = 0;
    g_data_counter = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!should_process_line(line, current_line)) {
            /* Do stuff */
        }
    }
}

bool should_process_line(char *line, int current_line) {
    if (line == NULL) {  /* Fake line :( */
        return false;
    }
    while (isspace(*line)) {  /* Skip white characters */
        line++;
        if (*line == ';' || *line == '\0') {  /* If comment or line's over */
            return false;
        }
        if (strlen(line) > MAX_LINE_LENGTH - 2) {
            g_error = ERROR_LINE_TOO_LONG;
            print_error(current_line);
            return false;
        }
    }
    return true;
}