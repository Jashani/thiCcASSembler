#include "first_pass.h"

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    g_instruction_counter = 0;
    g_data_counter = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        if (!should_process_line(line, current_line)) {
            success = process_line(line, current_line);
            if (!success) {
                print_error(current_line);
            }
        }

        current_line++;
    }
}

bool process_line(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH];
    bool has_label = false;
    directive directive_type;
    
    line = trim(line);
    if (line == NULL) {
        g_error = ERROR_FAILED_READ;
        return false;
    }

    if (!isalpha(*line) && *line != '.') {  /* Legal starting characters */
        g_error = ERROR_SYNTAX;
        return false;
    }

    if (check_for_label(line, &label)) {
        has_label = true;
    }
    else if (g_error != NO_ERRORS) {
        return false;
    }

    if (check_for_directive(line, &directive_type)) {
        handle_directive(line, directive_type, has_label);
    }
    else if (g_error != NO_ERRORS) {
        return false;
    }
}

bool check_for_label(char *line, char **label) {
    int index;
    int label_iterator = 0;
    while (*line != ':' && !isspace(*line) && *line != '\0' && label_iterator < MAX_LABEL_LENGTH) {
        *label[label_iterator] = *line;
        label_iterator++;
        line++;
    }
    *label[label_iterator] = '\0';

    while (*line != ':' && !isspace(*line) && *line != '\0') {  /* Push until the end of first field to determine potential issues */
        label_iterator++;
        line++;
    }

    if (*line != ':') {  /* No label */
        return false;
    }
    
    if (label_iterator > MAX_LABEL_LENGTH) {
        g_error = ERROR_LABEL_TOO_LONG;
        return false;
    }

    if (!isalpha(*label[0])) {  /* Label start must be alphabetical */
        g_error = ERROR_BAD_LABEL;
        return false;
    }

    for (index = 1; index < label_iterator; index++) {  /* Ensure proper format */
        if (!isalpha(*label[index]) && !isdigit(*label[index])) {
            g_error = ERROR_BAD_LABEL;
            return false;
        }
    }

    if (is_instruction(*label) || is_directive(*label) || is_register(*label)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    return true;
}

bool check_for_directive(char *line, int *directive_to_set) {
    if (*line != '.') {  /* Directives must start with a period! */
        return false;
    }

    if (match_word(line, "data", 4)) {
        *directive_to_set = DIRECTIVE_DATA;
        return true;
    }
    else if (match_word(line, "struct", 6)) {
        *directive_to_set = DIRECTIVE_STRUCT;
        return true;
    }
    else if (match_word(line, "string", 6)) {
        *directive_to_set = DIRECTIVE_STRING;
        return true;
    }
    else if (match_word(line, "entry", 5)) {
        *directive_to_set = DIRECTIVE_ENTRY;
        return true;
    }
    else if (match_word(line, "extern", 6)) {
        *directive_to_set = DIRECTIVE_EXTERNAL;
        return true;
    }

    g_error = ERROR_BAD_DIRECTIVE;
    return false;
}

bool handle_directive(char *line, directive directive_type, bool has_label) {
    char *current_field = line;

    current_field = next_field(current_field);
    if (current_field == NULL) {
        g_error = ERROR_MISSING_ARGUMENTS;
        return false;
    }

    if (directive_type == DIRECTIVE_DATA) {

    }

    else if (directive_type == DIRECTIVE_STRUCT) {

    }
    
    else if (directive_type == DIRECTIVE_STRING) {

    }

    else if (directive_type == DIRECTIVE_ENTRY) {

    }

    else if (directive_type == DIRECTIVE_EXTERNAL) {

    }
}

/* Return true if the first word in a line matches provided word, otherwise false. */
bool match_word(char *line, char *word, int word_length) {
    int real_length = 0;
    
    if (!strncmp(line, word, word_length)) {
        return false;
    }

    while (!isspace(*line) && *line != '\0') {
        real_length++;
        line++;
    }

    if (real_length == word_length) {
        return true;
    }
    
    return false;
}

char* next_field(char *line) {
    if (line == NULL)
        return NULL;
    while (!(isspace(*line)) && *line != '\0') {
        line++;
    }
    line = trim(line);
    if (line == '\0') {
        return NULL;
    }
    return line;
}

char* trim(char *line) {
    if (line == NULL || strlen(line) <= 1) {
        return NULL;
    }
    while (isspace(*line)) {
        line++;
    }
    return line;
}

bool should_process_line(char *line, int current_line) {
    if (line == NULL) {  /* Fake line :( */
        return false;
    }
    while (isspace(*line)) {  /* Skip white characters */
        line++;
        if (*line == ';' || *line == '\0') {  /* If comment or line's over */
            return false;
        }
        if (strlen(line) > MAX_LINE_LENGTH - 2) {
            g_error = ERROR_LINE_TOO_LONG;
            print_error(current_line);
            return false;
        }
    }
    return true;
}