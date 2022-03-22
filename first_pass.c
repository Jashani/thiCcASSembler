#include "first_pass.h"

bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success;

    g_instruction_counter = 100;
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
    print_data_image();
    print_code_image();
}

bool process_line(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH] = "\0";
    directive directive_type;
    int instruction;

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
    if (check_for_instruction(line, &instruction)) {
        return handle_instruction(line, instruction, label);
    } else {
        g_error = ERROR_BAD_INSTRUCTION;
        return false;
    }
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
        if (!add_symbol(label, g_data_counter, ATTRIBUTE_DATA)) { /* mock values */
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
    bool number_exists;
    int current_number;

    if (!isdigit(*line) && *line != '-' && *line != '+') {
        g_error = ERROR_SYNTAX;
        return false;
    }

    while (line != NULL && *line != '\0') {
        number_exists = false;
        sscanf(line, "%d", &current_number);

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

        add_to_data_image(set_to_base16(current_number));
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

    return true;
}

bool handle_directive_string(char *line) {    
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
        add_to_data_image(*line);
        line++;
        /* Extend data image */
    }
    add_to_data_image('\0'); /* For terminating character */

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

    return true;
}

bool handle_directive_extern(char *line) {
    char label[MAX_LABEL_LENGTH];
    extract_first_word(line, label);
    if (is_reserved(line)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    return add_symbol(label, 0, ATTRIBUTE_EXTERNAL);
}

bool handle_instruction(char *line, int instruction, char *label) {
    int arguments;
    char first_argument[MAX_LINE_LENGTH] = "\0";
    char second_argument[MAX_LINE_LENGTH] = "\0";
    bool has_label = (label[0] != '\0') ? true : false;

    printf("handling instruction\n");
    /* Add label to symbol table if exists */
    if (has_label && !add_symbol(label, g_instruction_counter, ATTRIBUTE_CODE)) {
        return false;
    }
    add_to_code_image(ENCODING_ABSOLUTE | instruction_opcode(instruction));
    /* Calculate size of instruction in image */
    arguments = instruction_arguments(instruction);
    line = next_field(line);
    if (arguments == 0 && (line == NULL || *line == '\0')) {
        return true;
    }

    break_arguments(line, first_argument, second_argument);
    if (!valid_line(line, arguments, first_argument, second_argument)) {
        return false;
    }

    return encode_instruction(arguments, instruction, first_argument, second_argument);
}

bool encode_instruction(int arguments, int instruction, 
                        char *first_argument, char *second_argument) {
    int first_addressing, second_addressing;
    char word_holder[MAX_LINE_LENGTH];
    int source_register = NO_VALUE, target_register = NO_VALUE;

    if (arguments >= 1) {
        first_addressing = addressing_method(first_argument);
        if (!is_addressing_legal(instruction, first_addressing, FIRST)) {
            g_error = ERROR_ADDRESSING;
            return false;
        }
    }

    if (arguments == 1) {
        if (first_addressing == ADDRESSING_INDEX) {
            word_in_brackets(first_argument, word_holder);
            target_register = register_to_value(word_holder);
        } else if (first_addressing == ADDRESSING_REGISTER) {
            target_register = register_to_value(first_argument);
        }

        add_to_code_image(build_binary_instruction(
            ENCODING_ABSOLUTE, instruction_functor(instruction),
            first_addressing, target_register, NO_VALUE, NO_VALUE));
        add_addressing_data(first_addressing, first_argument, FIRST);
    } else if (arguments == 2) {
        second_addressing = addressing_method(second_argument);
        if (!is_addressing_legal(instruction, second_addressing, SECOND)) {
            g_error = ERROR_ADDRESSING;
            return false;
        }

        if (first_addressing == ADDRESSING_INDEX) {
            word_in_brackets(first_argument, word_holder);
            source_register = register_to_value(word_holder);
        } else if (first_addressing == ADDRESSING_REGISTER) {
            source_register = register_to_value(first_argument);
        }
        if (second_addressing == ADDRESSING_INDEX) {
            word_in_brackets(first_argument, word_holder);
            target_register = register_to_value(word_holder);
        } else if (second_addressing == ADDRESSING_REGISTER) {
            target_register = register_to_value(second_argument);
        }

        add_to_code_image(build_binary_instruction(
            ENCODING_ABSOLUTE, instruction_functor(instruction),
            second_addressing, target_register, first_addressing,
            source_register));
        add_addressing_data(first_addressing, first_argument, FIRST);
        add_addressing_data(second_addressing, second_argument, SECOND);
    }

    return true;
}

bool add_addressing_data(addressing addressing_type, char *argument, int argument_slot) {
    if (addressing_type == ADDRESSING_IMMEDIATE) {
        add_to_code_image(ENCODING_ABSOLUTE | extract_immediate_value(argument));
    } else if (addressing_type == ADDRESSING_DIRECT ||
               addressing_type == ADDRESSING_INDEX) {
        add_to_code_image(0); /* Placeholders */
        add_to_code_image(0);
    }
}

/* Return the content of arguments of the format "arg1,arg2", cleaning spaces */
void break_arguments(char *line, char *first, char *second) {
    char temp_first[MAX_LINE_LENGTH] = "\0";
    char temp_second[MAX_LINE_LENGTH] = "\0";
    if (line == NULL) {
        return;
    }
    sscanf(line, "%[^,], %s", temp_first, temp_second);
    sscanf(temp_first, "%s", first);
    sscanf(temp_second, "%s", second);
}

/* Ensure line integrity */
bool valid_line(char *line, int arguments, char *first_argument, char *second_argument) {
    char temp1[MAX_LINE_LENGTH] = "\0";
    char temp2[MAX_LINE_LENGTH] = "\0";
    char temp3[MAX_LINE_LENGTH] = "\0";
    if ((arguments == 0 && (*first_argument != '\0' || *second_argument != '\0')) ||
        (arguments == 1 && (*first_argument == '\0' || *second_argument != '\0')) ||
        (arguments == 2 && (*first_argument == '\0' || *second_argument == '\0'))) {
        g_error = ERROR_ARGUMENT_STRUCTURE;
        return false;
    }

    /* Look for sneaky commas */
    break_arguments(second_argument, temp1, temp2);
    if (*temp2 != '\0') {
        g_error = ERROR_ARGUMENT_STRUCTURE;
        return false;
    }

    /* Too much content in the line */
    sscanf(line, "%s %s %s", temp1, temp2, temp3);
    if (*temp3 != '\0') {
        g_error = ERROR_ARGUMENT_STRUCTURE;
        return false;
    }

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