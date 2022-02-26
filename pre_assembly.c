#include "pre_assembly.h"

bool extract_first_word(char *line, char *result) {
    sscanf(line, "%s", result);
}

bool pre_assembly(FILE *original_file) {
    char line[MAX_LINE_LENGTH];
    char word[MAX_LINE_LENGTH];
    bool is_macro = false;

    extract_first_word(line, word);
    if (strcmp(word, "macro") == 0) {
        is_macro = true;
    }

    
}
