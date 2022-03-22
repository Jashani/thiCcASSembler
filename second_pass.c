#include "second_pass.h"

bool second_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        printf("\n--- Line %d ---\n", current_line);
        if (should_process_line(line, current_line)) {
            printf("Processing line %d\n", current_line);
            success = process_line(line, current_line);
            if (!success) {
                print_error(current_line);
            }
        } else {
            printf("Skipping line %d\n", current_line);
        }

        current_line++;
    }

    print_symbols();
    print_data_image();
    print_code_image();
}

bool process_line(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH] = "\0";
    directive directive_type;

    line = trim(line);
    if (!valid_start(line)) {
        return false;
    }

    printf("Checking for label\n");
    if (check_for_label(line, label)) {
        line = next_field(line);
        printf("Label is: %s\n", label);
    }

    printf("Checking for directive.\n");
    if (check_for_directive(line, &directive_type)) {
        return handle_directive(line, directive_type);
    }
}

bool handle_directive(char *line, directive directive_type) {
    if (directive_type != DIRECTIVE_ENTRY) {
        return true;
    }
}