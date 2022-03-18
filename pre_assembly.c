#include "pre_assembly.h"

bool pre_assembly(FILE *original_file) {
    char line[MAX_LINE_LENGTH];
    bool is_macro = false;

    while (fgets(line, MAX_LINE_LENGTH, original_file) != NULL) {
        if (match_word(line, "macro") == 0) {
            is_macro = true;
        }
    }


}
