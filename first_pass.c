#include "first_pass.h"

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

bool handle_directive(char *line, directive directive_type, char *label)
{
    char *current_field = line;

    current_field = next_field(current_field);
    if (current_field == NULL)
    {
        g_error = ERROR_MISSING_ARGUMENTS;
        return false;
    }

    if (directive_type == DIRECTIVE_DATA)
    {
        printf("Directive is data\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_STRUCT)
    {
        printf("Directive is struct\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_STRING)
    {
        printf("Directive is str\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_ENTRY)
    {
        printf("Directive is ent\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_EXTERNAL)
    {
        printf("Directive is ext\n");
        return true;
    }
}

bool handle_instruction(char *line, int instruction_index, char *label)
{
    printf("handling instruction\n");
    return true;
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