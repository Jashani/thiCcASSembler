#include "first_pass.h"

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;

    instruction_counter = 0;
    data_counter = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!should_process_line(line)) {
            /* Do stuff */
        }
    }
}

bool should_process_line(char *line) {
    if (line == NULL) {  /* Fake line :( */
        return false;
    }
    while (isspace(*line)) {  /* Skip white characters */
        line++;
        if (*line == ';' || *line == '\0') {  /* If comment or line's over */
            return false;
        }
    }
    return true;
}