#include "first_pass.h"

/* Run first pass on provided file. */
bool first_pass(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    bool success, absolute_success = true;

    g_instruction_counter = 100;
    g_data_counter = 0;

    /* Process all lines in file */
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        g_error = NO_ERRORS;
        if (should_process_line(line, current_line)) {
            success = process_line(line, current_line);
            if (!success) {
                absolute_success = false;
                print_error(current_line);
            }
        }
        current_line++;
    }

    /* Update image values */
    finalise_data();
    return absolute_success;
}

/* Determine whether line contains label, directive, or instruction.
 * Act accordingly.
 */
bool process_line(char *line, int current_line) {
    char label[MAX_LABEL_LENGTH] = "\0";
    directive directive_type;
    int instruction;

    /* Clean the start of the line and validate its integrity */
    line = trim(line);
    if (!valid_start(line)) {
        return false;
    }

    /* If label exists, save it and continue into the line */
    if (check_for_label(line, label)) {
        line = next_field(line);
    } else if (g_error != NO_ERRORS) {
        return false;
    }

    if (check_for_directive(line, &directive_type)) {
        return handle_directive(line, directive_type, label);
    } else if (g_error != NO_ERRORS) {
        return false;
    }

    if (check_for_instruction(line, &instruction)) {
        return handle_instruction(line, instruction, label);
    } else {
        g_error = ERROR_BAD_INSTRUCTION;
        return false;
    }

    /* Line successfully processed */
    return true;
}

/* Attach label to directive if exists, then process directive appropriately. */
bool handle_directive(char *line, directive directive_type, char *label) {
    char *current_field = line;
    bool has_label = (label[0] != '\0') ? true : false; /* For legibility */

    /* Read directive arguments */
    current_field = next_field(current_field);
    if (current_field == NULL) {
        g_error = ERROR_MISSING_ARGUMENTS;
        return false;
    }

    /* Add symbol for directive if label exists */
    if (has_label) {
        if (directive_type != DIRECTIVE_DATA && directive_type != DIRECTIVE_STRING) {
            g_error = ERROR_CANNOT_LABEL_DIRECTIVE;
            return false;
        }
        if (!add_symbol(label, g_data_counter, ATTRIBUTE_DATA)) {
            return false;
        }
    }

    if (directive_type == DIRECTIVE_DATA) {
        return handle_directive_data(current_field);
    }

    else if (directive_type == DIRECTIVE_STRING) {
        return handle_directive_string(current_field);
    }

    else if (directive_type == DIRECTIVE_ENTRY) {
        /* Will be handled during second pass */
        return true;
    }

    else if (directive_type == DIRECTIVE_EXTERNAL) {
        handle_directive_extern(current_field);
        return true;
    }
}

/* Handle directive of type data;
 * Ensure each integer fits appropriate format, add each to data image */
bool handle_directive_data(char *line) {
    bool number_exists;
    int current_number;

    /* Ensure legal data start */
    if (!isdigit(*line) && *line != '-' && *line != '+') {
        g_error = ERROR_SYNTAX;
        return false;
    }

    /* Collect all integers in line */
    while (line != NULL && *line != '\0') {
        number_exists = false;
        sscanf(line, "%d", &current_number);

        if (*line == '-' || *line == '+') {
            line++;
        }

        /* Ensure there is actually a number in the line */
        while (isdigit(*line)) {
            number_exists = true;
            line++;
        }

        if (!number_exists) {
            g_error = ERROR_SYNTAX;
            return false;
        }

        /* Add to data image and continue to the next number */
        add_to_data_image(set_to_size16(current_number));
        line = trim(line);
        if (line == NULL || *line == '\0') {
            break;
        }

        /* Ensure legal line continuation */
        if (*line != ',' || *(trim(line + 1)) == '\0') {
            g_error = ERROR_SYNTAX;
            return false;
        }

        line++;
        line = trim(line);
    }

    return true;
}

/* Ensure  line is wrapped with quotes ("...") and add string to data image */
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

    /* Add each character to data image */
    while (line != NULL && *line != '"' && *line != '\0') {
        add_to_data_image(*line);
        line++;
    }
    add_to_data_image('\0'); /* For terminating character */

    if (line == NULL || *line == '\0') {
        g_error = ERROR_MISSING_QUOTES;
        return false;
    }

    /* Validate line integrity */
    line++;
    line = trim(line);
    if (line != NULL && *line != '\0') {
        g_error = ERROR_SYNTAX;
        return false;
    }

    return true;
}

/* Add extern symbol with empty values */
bool handle_directive_extern(char *line) {
    char label[MAX_LABEL_LENGTH];
    extract_first_word(line, label);
    if (is_reserved(line)) {
        g_error = ERROR_RESERVED_KEYWORD;
        return false;
    }

    return add_symbol(label, 0, ATTRIBUTE_EXTERNAL);
}

/* Parse instruction and encode to code image */
bool handle_instruction(char *line, int instruction, char *label) {
    int arguments;
    char first_argument[MAX_LINE_LENGTH] = "\0";
    char second_argument[MAX_LINE_LENGTH] = "\0";
    bool has_label = (label[0] != '\0') ? true : false; /* For legibility */

    printf("handling instruction\n");
    /* Add label to symbol table if exists */
    if (has_label && !add_symbol(label, g_instruction_counter, ATTRIBUTE_CODE)) {
        return false;
    }
    add_to_code_image(ENCODING_ABSOLUTE | instruction_opcode(instruction), NULL);
    /* Calculate size of instruction in image */
    arguments = instruction_arguments(instruction);
    line = next_field(line);
    if (arguments == 0 && (line == NULL || *line == '\0')) {
        return true;
    }

    /* Split arguments given to instruction */
    break_arguments(line, first_argument, second_argument);
    if (!valid_line(line, arguments, first_argument, second_argument)) {
        return false;
    }

    return encode_instruction(arguments, instruction, first_argument, second_argument);
}

/* Format instruction and arguments, then add to code image */
bool encode_instruction(int arguments, int instruction, 
                        char *first_argument, char *second_argument) {
    int first_addressing, second_addressing;
    char word_holder[MAX_LINE_LENGTH];
    int source_register = NO_VALUE, target_register = NO_VALUE;

    /* Determine and validate addressing method for first argument */
    if (arguments >= 1) {
        first_addressing = addressing_method(first_argument);
        if (!is_addressing_legal(instruction, first_addressing, FIRST)) {
            g_error = ERROR_ADDRESSING;
            return false;
        }
    }

    /* Encode instruction */
    if (arguments == 1) {
        if (first_addressing == ADDRESSING_INDEX) {
            /* Parse index addressing */
            word_in_brackets(first_argument, word_holder);
            target_register = register_to_value(word_holder);
        } else if (first_addressing == ADDRESSING_REGISTER) {
            target_register = register_to_value(first_argument);
        }

        add_to_code_image(build_binary_instruction(
            ENCODING_ABSOLUTE, instruction_functor(instruction),
            first_addressing, target_register, NO_VALUE, NO_VALUE), NULL);
        add_addressing_data(first_addressing, first_argument, FIRST);
    } else if (arguments == 2) {
        /* Determine and validate addressing method for second argument */
        second_addressing = addressing_method(second_argument);
        if (!is_addressing_legal(instruction, second_addressing, SECOND)) {
            g_error = ERROR_ADDRESSING;
            return false;
        }

        if (first_addressing == ADDRESSING_INDEX) {
            /* Parse index addressing */
            word_in_brackets(first_argument, word_holder);
            source_register = register_to_value(word_holder);
        } else if (first_addressing == ADDRESSING_REGISTER) {
            source_register = register_to_value(first_argument);
        }
        if (second_addressing == ADDRESSING_INDEX) {
            /* Parse index addressing */
            word_in_brackets(first_argument, word_holder);
            target_register = register_to_value(word_holder);
        } else if (second_addressing == ADDRESSING_REGISTER) {
            target_register = register_to_value(second_argument);
        }

        add_to_code_image(build_binary_instruction(
            ENCODING_ABSOLUTE, instruction_functor(instruction),
            second_addressing, target_register, first_addressing,
            source_register), NULL);
        add_addressing_data(first_addressing, first_argument, FIRST);
        add_addressing_data(second_addressing, second_argument, SECOND);
    }

    return true;
}

/* Add further data for relevant addressing type */
bool add_addressing_data(addressing addressing_type, char *argument, int argument_slot) {
    char potential_label[MAX_LABEL_LENGTH] = "\0";
    if (addressing_type == ADDRESSING_IMMEDIATE) {
        add_to_code_image(ENCODING_ABSOLUTE | extract_immediate_value(argument), NULL);
    } else if (addressing_type == ADDRESSING_DIRECT ||
               addressing_type == ADDRESSING_INDEX) {
        word_before_brackets(argument, potential_label);
        add_to_code_image(0, potential_label);
        add_to_code_image(0, potential_label);
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

/* Extract value for immediate addressing */
int extract_immediate_value(char *argument) {
    int value;
    sscanf(argument, "#%d", &value);
    /* Ensure proper format */
    value = set_to_size16(value);
    return value;
}