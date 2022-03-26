#include "pre_assembly.h"

/* Unfold macros through file */
bool pre_assembly(FILE *source_file, FILE *expanded_source_file) {
    char line[MAX_LINE_LENGTH];
    char potential_macro[MAX_LINE_LENGTH];
    char current_name[MAX_LINE_LENGTH];
    char *untrimmed_name = NULL;
    char *current_content = calloc(1, 1); /* Allocating an empty string */
    bool is_macro = false;
    int current_line = 0;
    bool success = true;

    /* Read through whole file */
    while (fgets(line, MAX_LINE_LENGTH, source_file) != NULL) {
        current_line++;
        /* if first word is macro found in macro table, expand it and continue */
        extract_first_word(line, potential_macro);
        if (macro_exists(potential_macro)) {
            fprintf(expanded_source_file, "%s", get_macro(potential_macro));
            continue;
        }

        /* Keep adding to macro content until 'endm' */
        if (is_macro) {
            /* Once found endm, finalise macro and add to table */
            if (match_word(line, "endm")) {
                is_macro = false;
                add_macro(current_name, current_content);
                free(current_content);
                current_content = "";
                continue;
            }
            current_content = extend(current_content, line);
            continue;
        }

        /* Initialise macro recording process */
        else if (match_word(line, "macro")) {
            untrimmed_name = next_field(trim(line));
            extract_first_word(untrimmed_name, current_name);
            if (is_reserved(current_name)) {
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
