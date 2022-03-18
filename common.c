#include "common.h"

char* concatenate(char *start, char *end) {
    const int NULL_INDEX = 1;
    char *result = (char *) safe_malloc(strlen(start) + strlen(end) + NULL_INDEX);
    strcpy(result, start);
    strcat(result, end);
    return result;
}

void *safe_malloc(long size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        printf("Failed to allocate memory, exiting.");
        exit(FAILURE);
    }
    return pointer;
}

bool extract_first_word(char *line, char *result) {
    sscanf(line, "%s", result);
}

/* Return true if the first word in a line matches provided word, otherwise false. */
bool match_word(char *line, char *word) {
    char first_word[MAX_LINE_LENGTH];
    extract_first_word(line, first_word);
    if (strcmp(first_word, word) == 0) {
        return true;
    }
    return false;
}