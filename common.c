#include "common.h"

char *concatenate(char *start, char *end) {
    const int NULL_INDEX = 1;
    char *result = (char *)safe_malloc(strlen(start) + strlen(end) + NULL_INDEX);
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

void extract_first_word(char *line, char *result) {
    sscanf(line, "%s", result);
}

/* Return true if the first word in a line matches provided word, otherwise false. */
bool match_word(char *line, char *word) {
    char first_word[MAX_LINE_LENGTH];
    extract_first_word(line, first_word);
    if (strings_match(first_word, word)) {
        return true;
    }
    return false;
}

char *next_field(char *line) {
    if (line == NULL) {
        return NULL;
    }
    while (!(isspace(*line)) && *line != '\0') {
        line++;
    }

    line = trim(line);
    if (*line == '\0') {
        return NULL;
    }
    return line;
}

char *trim(char *line) {
    if (line == NULL || strlen(line) <= 1) {
        return NULL;
    }
    while (isspace(*line)) {
        line++;
    }
    return line;
}

bool strings_match(char *first, char *second) {
    char temp;
    if (first == NULL || second == NULL) {
        return false;
    }

    do {
        temp = *first;
        if (temp == '\n') {
            temp = '\0';
        }
        if (temp != *second) {
            return false;
        }
        first++;
        second++;
    } while (temp);
    return true;
}