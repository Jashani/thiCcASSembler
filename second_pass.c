#include "second_pass.h"

/* Perform second pass on file */
bool second_pass(FILE *file) {
    pass_over_file(file);
    if (!populate_symbols()) {
        print_error(UNKNOWN_LINE);
        return false;
    }

    return true;
}

bool pass_over_file(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    /* Read whole file and process relevant lines */
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        if (should_process_line(line, current_line)) {
            success = process_line_again(line, current_line);
            if (!success) {
                print_error(current_line);
            }
        }

        current_line++;
    }
}

/* Second pass processing of line */
bool process_line_again(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH] = "\0";
    directive directive_type;

    line = trim(line);
    /* Validate line integrity */
    if (!valid_start(line)) {
        return false;
    }

    /* Ignore labels */
    if (check_for_label(line, label)) {
        line = next_field(line);
    }

    if (check_for_directive(line, &directive_type)) {
        return handle_directive_again(line, directive_type);
    }

    return true;
}

/* We only care of entry labels. If found, add entry atribute to relevant symbol */
bool handle_directive_again(char *line, directive directive_type) {
    if (directive_type != DIRECTIVE_ENTRY) {
        return true;
    }
    line = next_field(line);
    return add_attribute_to_symbol(line, ATTRIBUTE_ENTRY);
}