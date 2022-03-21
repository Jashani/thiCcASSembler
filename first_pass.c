#include "first_pass.h"

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    g_instruction_counter = 0;
    g_data_counter = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        printf("\n--- Line %d ---\n", current_line);
        if (should_process_line(line, current_line)) {
            printf("Processing line %d\n", current_line);
            success = process_line(line, current_line);
            if (!success) {
                print_error(current_line);
            }
        } else {
            printf("Skipping line %d\n", current_line);
        }

        current_line++;
    }

    printf("Data counter: %d\n", g_data_counter);
    print_symbols();
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

    if (!isalpha(*line) && *line != '.') { /* Legal starting characters */
        g_error = ERROR_SYNTAX;
        return false;
    }

    printf("Checking for label\n");
    if (check_for_label(line, label)) {
        line = next_field(line);
        printf("Label is: %s\n", label);
    } else if (g_error != NO_ERRORS) {
        return false;
    }

    printf("Checking for directive.\n");
    if (check_for_directive(line, &directive_type)) {
        return handle_directive(line, directive_type, label);
    } else if (g_error != NO_ERRORS) {
        return false;
    }

    printf("Checking for instruction\n");
    if (check_for_instruction(line, &instruction_index)) {
        return handle_instruction(line, instruction_index, label);
    } else {
        g_error = ERROR_BAD_INSTRUCTION;
        return false;
    }
}

bool check_for_label(char *line, char *label) {
    int index;
    int label_iterator = 0;
    char temp_label[MAX_LABEL_LENGTH];

    while (*line != ':' && !isspace(*line) && *line != '\0' &&
           label_iterator < MAX_LABEL_LENGTH) {
        temp_label[label_iterator] = *line;
        label_iterator++;
        line++;
    }
    temp_label[label_iterator] = '\0';

    /* Push until the end of first field to determine potential issues */
    while (*line != ':' && !isspace(*line) && *line != '\0') {
        label_iterator++;
        line++;
    }

    if (*line != ':') { /* No label */
        return false;
    }

    if (label_iterator > MAX_LABEL_LENGTH) {
        g_error = ERROR_LABEL_TOO_LONG;
        return false;
    }

    if (!isalpha(temp_label[0])) { /* Label start must be alphabetical */
        g_error = ERROR_BAD_LABEL;
        return false;
    }

    for (index = 1; index < label_iterator; index++) { /* Ensure proper format */
        if (!isalpha(temp_label[index]) && !isdigit(temp_label[index])) {
            g_error = ERROR_BAD_LABEL;
            return false;
        }
    }

    if (is_reserved(temp_label)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    strcpy(label, temp_label);
    return true;
}

bool handle_directive(char *line, directive directive_type, char *label) {
    char *current_field = line;
    bool has_label = (label[0] != '\0') ? true : false;
    printf("Handling directive.\n");

    current_field = next_field(current_field);
    if (current_field == NULL) {
        g_error = ERROR_MISSING_ARGUMENTS;
        return false;
    }

    if (has_label) {
        if (directive_type != DIRECTIVE_DATA && directive_type != DIRECTIVE_STRING) {
            g_error = ERROR_CANNOT_LABEL_DIRECTIVE;
            return false;
        }
        if (!add_symbol(label, g_data_counter, 1, 1, ATTRIBUTE_DATA)) { /* mock values */
            return false;
        }
    }

    if (directive_type == DIRECTIVE_DATA) {
        /* Add to symbol table with attribute 'data' */
        /* Add to image */
        printf("Directive is data\n");
        return handle_directive_data(current_field);
    }

    else if (directive_type == DIRECTIVE_STRING) {
        /* Add to symbol table with attribute 'data' */
        /* Add to image */
        printf("Directive is str\n");
        return handle_directive_string(current_field);
    }

    else if (directive_type == DIRECTIVE_ENTRY) {
        /* Will be handled during second pass */
        printf("Directive is ent\n");
        return true;
    }

    else if (directive_type == DIRECTIVE_EXTERNAL) {
        printf("Directive is ext\n");
        handle_directive_extern(current_field);
        return true;
    }
}

bool handle_directive_data(char *line) {
    int temp_count = 0;
    bool number_exists;

    if (!isdigit(*line) && *line != '-' && *line != '+') {
        g_error = ERROR_SYNTAX;
        return false;
    }

    while (line != NULL && *line != '\0') {
        number_exists = false;

        if (*line == '-' || *line == '+') {
            line++;
        }

        while (isdigit(*line)) {
            number_exists = true;
            line++;
        }

        if (!number_exists) {
            g_error = ERROR_SYNTAX;
            return false;
        }

        temp_count++;
        line = trim(line);
        if (line == NULL || *line == '\0') {
            break;
        }

        if (*line != ',' || *(trim(line + 1)) == '\0') {
            g_error = ERROR_SYNTAX;
            return false;
        }

        line++;
        line = trim(line);
    }

    g_data_counter += temp_count;
    return true;
}

bool handle_directive_string(char *line) {
    int temp_count = 0;
    
    if (*line != '"') {
        g_error = ERROR_MISSING_QUOTES;
        return false;
    }

    line++;
    if (*line == '"') {
        g_error = ERROR_EMPTY_STRING;
        return false;
    }

    while (line != NULL && *line != '"' && *line != '\0') {
        line++;
        temp_count++;
        /* Extend data image */
    }

    temp_count++; /* For terminating character */
    if (line == NULL || *line == '\0') {
        g_error = ERROR_MISSING_QUOTES;
        return false;
    }

    line++;
    line = trim(line);
    if (line != NULL && *line != '\0') {
        g_error = ERROR_SYNTAX;
        return false;
    }

    g_data_counter += temp_count;
    return true;
}

bool handle_directive_extern(char *line) {
    char label[MAX_LABEL_LENGTH];
    extract_first_word(line, label);
    if (is_reserved(line)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    return add_symbol(label, 0, 0, 0, ATTRIBUTE_EXTERNAL);
}

bool handle_instruction(char *line, int instruction_index, char *label) {
    attribute_set attributes = ATTRIBUTE_NONE;
    bool has_label = (label[0] != '\0') ? true : false;

    if (has_label) {
        attributes = ATTRIBUTE_CODE;
        if (!add_symbol(label, 1, 1, 1, attributes)) { /* mock values */
            return false;
        }
    }

    printf("handling instruction\n");
    return true;
}

bool should_process_line(char *line, int current_line) {
    line = trim(line);
    if (line == NULL) { /* Fake line :( */
        return false;
    }
    printf("Line %d is:\n|\t%s\n", current_line, line);
    if (*line == ';' || *line == '\0') { /* If comment or line's over */
        return false;
    }
    if (strlen(line) > MAX_LINE_LENGTH - 2) {
        g_error = ERROR_LINE_TOO_LONG;
        print_error(current_line);
        return false;
    }
    return true;
}