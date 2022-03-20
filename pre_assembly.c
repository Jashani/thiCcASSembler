#include "pre_assembly.h"

bool pre_assembly(FILE *source_file, FILE *expanded_source_file) {
    char line[MAX_LINE_LENGTH];
    char potential_macro[MAX_LINE_LENGTH];
    char current_name[MAX_LINE_LENGTH];
    char *untrimmed_name;
    char *current_content = "";
    bool is_macro = false;
    int current_line = 0;
    bool success = true;

    while (fgets(line, MAX_LINE_LENGTH, source_file) != NULL) {
        current_line++;
        /* if first word is macro found in macro table, expand it and continue */
        extract_first_word(line, potential_macro);
        if (macro_exists(potential_macro)) {
            fprintf(expanded_source_file, "%s", get_macro(potential_macro));
            continue;
        }

        if (is_macro) {
            if (match_word(line, "endmacro")) {
                is_macro = false;
                add_macro(current_name, current_content);
                current_content = "";
                continue;
            }
            current_content = concatenate(current_content, line);
        }

        else if (match_word(line, "macro")) {
            untrimmed_name = next_field(trim(line));
            extract_first_word(untrimmed_name, current_name);
            if (is_directive(current_name) || is_instruction(current_name) || is_register(current_name)) {
                g_error = ERROR_RESERVED_KEYWORD;
                print_error(current_line);
                success = false;
                continue;
            }
            is_macro = true;
            continue;
        }

        fprintf(expanded_source_file, "%s", line);
    }

    clear_macros();
    return success;
}
