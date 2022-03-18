#include "first_pass.h"

char *instructions[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    g_instruction_counter = 0;
    g_data_counter = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        printf("\n--- Line %d ---\n", current_line);
        if (should_process_line(line, current_line)) {
            printf("Processing line %d\n", current_line);
            success = process_line(line, current_line);
            if (!success) {
                print_error(current_line);
            }
        }
        else {
            printf("Skipping line %d\n", current_line);
        }

        current_line++;
    }
}

bool process_line(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH] = "\0";
    directive directive_type;
    int instruction_index;
    
    line = trim(line);
    if (line == NULL) {
        g_error = ERROR_FAILED_READ;
        return false;
    }

    if (!isalpha(*line) && *line != '.') {  /* Legal starting characters */
        g_error = ERROR_SYNTAX;
        return false;
    }

    printf("Checking for label\n");
    if (check_for_label(line, label)) {
        line = next_field(line);
        printf("Label is: %s\n", label);
    }
    else if (g_error != NO_ERRORS) {
        return false;
    }

    printf("Checking for directive\n");
    if (check_for_directive(line, &directive_type)) {
        handle_directive(line, directive_type, label);
        return true;
    }
    else if (g_error != NO_ERRORS) {
        return false;
    }

    printf("Checking for instruction\n");
    if (check_for_instruction(line, &instruction_index)) {
        handle_instruction(line, instruction_index, label);
        return true;
    }
    else {
        g_error = ERROR_BAD_INSTRUCTION;
        return false;
    }
}

bool check_for_label(char *line, char *label) {
    int index;
    int label_iterator = 0;
    while (*line != ':' && !isspace(*line) && *line != '\0' && label_iterator < MAX_LABEL_LENGTH) {
        label[label_iterator] = *line;
        label_iterator++;
        line++;
    }
    label[label_iterator] = '\0';

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

    if (!isalpha(label[0])) {  /* Label start must be alphabetical */
        g_error = ERROR_BAD_LABEL;
        return false;
    }

    for (index = 1; index < label_iterator; index++) {  /* Ensure proper format */
        if (!isalpha(label[index]) && !isdigit(label[index])) {
            g_error = ERROR_BAD_LABEL;
            return false;
        }
    }

    if (is_instruction(label) || is_directive(label) || is_register(label)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    return true;
}

bool check_for_directive(char *line, directive *directive_to_set) {
    if (*line != '.') {  /* Directives must start with a period! */
        return false;
    }

    line++;
    if (match_word(line, "data")) {
        *directive_to_set = DIRECTIVE_DATA;
        return true;
    }
    else if (match_word(line, "struct")) {
        *directive_to_set = DIRECTIVE_STRUCT;
        return true;
    }
    else if (match_word(line, "string")) {
        *directive_to_set = DIRECTIVE_STRING;
        return true;
    }
    else if (match_word(line, "entry")) {
        *directive_to_set = DIRECTIVE_ENTRY;
        return true;
    }
    else if (match_word(line, "extern")) {
        *directive_to_set = DIRECTIVE_EXTERNAL;
        return true;
    }

    g_error = ERROR_BAD_DIRECTIVE;
    return false;
}

bool handle_directive(char *line, directive directive_type, char *label) {
    char *current_field = line;

    current_field = next_field(current_field);
    if (current_field == NULL) {
        g_error = ERROR_MISSING_ARGUMENTS;
        return false;
    }

    if (directive_type == DIRECTIVE_DATA) {
        printf("Directive is data\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_STRUCT) {
        printf("Directive is struct\n");
        return true;
    }
    
    else if (directive_type == DIRECTIVE_STRING) {
        printf("Directive is str\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_ENTRY) {
        printf("Directive is ent\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_EXTERNAL) {
        printf("Directive is ext\n");
        return true;
    }
}

bool check_for_instruction(char *line, int *instruction_index) {
    int index;
    for (index = 0; index < INSTRUCTION_COUNT; index++) {
        if (match_word(line, instructions[index])) {
            *instruction_index = index;
            return true;
        }
    }
    return false;
}

bool handle_instruction(char *line, int instruction_index, char *label) {
    printf("handling instruction\n");
    return true;
}

bool is_instruction(char *term) {
    int placeholder;
    return check_for_instruction(term, &placeholder);
}

bool is_directive(char *term) {
    bool result;
    directive placeholder;
    char *term_with_period = concatenate(".", term);
    result = check_for_directive(term_with_period, &placeholder);
    if (g_error == ERROR_BAD_DIRECTIVE) { 
        g_error = NO_ERRORS;  /* Unset bad directive since we're just looking for the term */
    }
    free(term_with_period);
    return result;
}

bool is_register(char *term) {
    int index;
    for (index = 0; index < REGISTER_COUNT; index++) {
        if (match_word(term, instructions[index])) {
            return true;
        }
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
    if (*line == '\0') {
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
    line = trim(line);
    if (line == NULL) {  /* Fake line :( */
        return false;
    }
    printf("Line %d is:\n\t%s\n", current_line, line);
    if (*line == ';' || *line == '\0') {  /* If comment or line's over */
        return false;
    }
    if (strlen(line) > MAX_LINE_LENGTH - 2) {
        g_error = ERROR_LINE_TOO_LONG;
        print_error(current_line);
        return false;
    }
    return true;
}