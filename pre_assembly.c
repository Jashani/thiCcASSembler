#include "pre_assembly.h"

bool pre_assembly(FILE *original_file) {
    char line[MAX_LINE_LENGTH];
    char current_name[MAX_LABEL_LENGTH];
    char *current_content = "";
    bool is_macro = false;

    while (fgets(line, MAX_LINE_LENGTH, original_file) != NULL) {
        /* if first word is macro found in macro table, do stuff */

        if (is_macro) {
            if (match_word(line, "endm")) {
                is_macro = false;
                /* delete line from file*/
                add_macro(current_name, current_content);
                current_content = "";
                continue;
            }
            current_content = concatenate(current_content, line);
        }

        else if (match_word(line, "macro")) {
            is_macro = true;
        }
    }


}
