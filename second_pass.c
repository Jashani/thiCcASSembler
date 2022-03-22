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