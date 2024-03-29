#include "utilities.h"

/* Concatenante two terms together */
char *concatenate(char *start, char *end) {
    const int NULL_INDEX = 1;
    char *result =
        (char *)safe_malloc(strlen(start) + strlen(end) + NULL_INDEX);
    strcpy(result, start);
    strcat(result, end);
    return result;
}

/* Same as concatanate, but free 'current', assuming the usage x = extend(x, y) */
char *extend(char *current, char *extra) {
    const int NULL_INDEX = 1;
    char *result =
        (char *)safe_malloc(strlen(current) + strlen(extra) + NULL_INDEX);
    strcpy(result, current);
    strcat(result, extra);
    free(current);
    return result;
}

/* Malloc + check */
void *safe_malloc(long size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        printf("Failed to allocate memory, exiting.");
        exit(FAILURE);
    }
    return pointer;
}

/* Return the first word in a line */
void extract_first_word(char *line, char *result) {
    sscanf(line, "%s", result);
}

/* Return true if the first word in a line matches provided word, otherwise
 * false. */
bool match_word(char *line, char *word) {
    char first_word[MAX_LINE_LENGTH];
    extract_first_word(line, first_word);
    if (strings_match(first_word, word)) {
        return true;
    }
    return false;
}

/* Skip to next field in a line */
char *next_field(char *line) {
    if (line == NULL) {
        return NULL;
    }
    while (line != NULL && !(isspace(*line)) && *line != '\0') {
        line++;
    }

    line = trim(line);
    if (line == NULL || *line == '\0') {
        return NULL;
    }
    return line;
}

/* Trim whitespaces before line */
char *trim(char *line) {
    if (line == NULL || strlen(line) <= 1) {
        return NULL;
    }
    while (isspace(*line)) {
        line++;
    }
    return line;
}

/* Return wether strings are identical */
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

/* Return content in brackets */
void word_in_brackets(char *line, char *word) {
    sscanf(line, "%*[^[][%3s]", word);
}

/* Return content before brackets */
void word_before_brackets(char *line, char *word) {
    char temp_word[MAX_LINE_LENGTH];
    sscanf(line, "%[^[]", temp_word); /* Word before brackets */
    sscanf(temp_word, "%s", word);    /* Trim both sides */
}

int set_to_size16(int value) {
    return (value & 0x0000FFFF);
}

/* Return whether line should be processed */
bool should_process_line(char *line, int current_line) {
    line = trim(line);

     /* Fake line :( */
    if (line == NULL) {
        return false;
    }

    /* If comment or line's over */
    if (*line == ';' || *line == '\0') {
        return false;
    }

    /* Line is too long! */
    if (strlen(line) > MAX_LINE_LENGTH - 2) {
        g_error = ERROR_LINE_TOO_LONG;
        print_error(current_line);
        return false;
    }
    return true;
}

/* Ensure start of line is valid */
bool valid_start(char *line) {
    if (line == NULL) {
        g_error = ERROR_FAILED_READ;
        return false;
    }

     /* Legal starting characters */
    if (!isalpha(*line) && *line != '.') {
        g_error = ERROR_SYNTAX;
        return false;
    }
    
    return true;
}