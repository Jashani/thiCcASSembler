#include "term_detection.h"

#ifndef CONSTANT_ARRAYS
#define CONSTANT_ARRAYS

char *instructions[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                        "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

/* Opcodes and functors per instruction */
int instruction_opcodes[] = {0, 1, 2, 2, 4, 5, 5, 5, 5, 9, 9, 9, 12, 13, 14, 15};
int instruction_functors[] = {0, 0, 10, 11, 0, 10, 11, 12, 13, 10, 11, 12, 0, 0, 0, 0};

/* Addressing methods per instruction, per argument */
addressing first_argument_addressing[] = {
    ADDRESSING_ALL,      ADDRESSING_ALL,      ADDRESSING_ALL,
    ADDRESSING_ALL,      ADDRESSING_VARIABLE, ADDRESSING_DEFERRED,
    ADDRESSING_DEFERRED, ADDRESSING_DEFERRED, ADDRESSING_DEFERRED,
    ADDRESSING_VARIABLE, ADDRESSING_VARIABLE, ADDRESSING_VARIABLE,
    ADDRESSING_DEFERRED, ADDRESSING_ALL,      ADDRESSING_NONE,
    ADDRESSING_NONE};
addressing second_argument_addressing[] = {
    ADDRESSING_DEFERRED, ADDRESSING_ALL,      ADDRESSING_DEFERRED,
    ADDRESSING_DEFERRED, ADDRESSING_DEFERRED, ADDRESSING_NONE,
    ADDRESSING_NONE,     ADDRESSING_NONE,     ADDRESSING_NONE,
    ADDRESSING_NONE,     ADDRESSING_NONE,     ADDRESSING_NONE,
    ADDRESSING_NONE,     ADDRESSING_NONE,     ADDRESSING_NONE,
    ADDRESSING_NONE};

char *registers[] = {"r0", "r1",  "r2",  "r3",  "r4",  "r5",  "r6", "r7",
                     "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
/* Registers allowed for usage with indexing operations */
char *index_registers[] = {"r10", "r11", "r12", "r13", "r14", "r15"};

#endif

/* Check if label in line */
bool check_for_label(char *line, char *label) {
    int index;
    int label_iterator = 0;
    char temp_label[MAX_LABEL_LENGTH];

    /* Collect suspected label */
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

     /* No label */
    if (*line != ':') {
        return false;
    }

    if (label_iterator > MAX_LABEL_LENGTH) {
        g_error = ERROR_LABEL_TOO_LONG;
        return false;
    }

    /* Label start must be alphabetical */
    if (!isalpha(temp_label[0])) {
        g_error = ERROR_BAD_LABEL;
        return false;
    }

     /* Ensure proper format */
    for (index = 1; index < label_iterator; index++) {
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

/* Check whether line contains directive */
bool check_for_directive(char *line, directive *directive_to_set) {
    if (*line != '.') { /* Directives must start with a period! */
        return false;
    }

    line++;
    if (match_word(line, "data")) {
        *directive_to_set = DIRECTIVE_DATA;
        return true;
    } else if (match_word(line, "string")) {
        *directive_to_set = DIRECTIVE_STRING;
        return true;
    } else if (match_word(line, "entry")) {
        *directive_to_set = DIRECTIVE_ENTRY;
        return true;
    } else if (match_word(line, "extern")) {
        *directive_to_set = DIRECTIVE_EXTERNAL;
        return true;
    }

    /* If we passed the period check and got here, the directive specification is bad */
    g_error = ERROR_BAD_DIRECTIVE;
    return false;
}

/* Check if instruction in line, return instruction index value if so */
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

/* Convert register name to value, i.e. char* "r8" -> int 8 */
int register_to_value(char *register_name) {
    int index;
    for (index = 0; index < REGISTER_COUNT; index++) {
        if (match_word(register_name, registers[index])) {
            return index;
        }
    }
}

/* Return opcode for instruction */
int instruction_opcode(int index) {
    int binary_opcode = 1 << instruction_opcodes[index];
    return binary_opcode;
}

/* Return functor for instruction */
int instruction_functor(int index) {
    return instruction_functors[index];
}

/* Return argument count for instruction */
int instruction_arguments(int index) {
    if (index <= 4) {
        return 2;
    } else if (index >= 14) {
        return 0;
    } else {
        return 1;
    }
}

/* Return whether term is instruction */
bool is_instruction(char *term) {
    int placeholder;
    return check_for_instruction(term, &placeholder);
}

/* Return whether term is directive */
bool is_directive(char *term) {
    bool result;
    directive placeholder;
    char *term_with_period = concatenate(".", term);
    result = check_for_directive(term_with_period, &placeholder);
    if (g_error == ERROR_BAD_DIRECTIVE) {
        g_error = NO_ERRORS; /* Unset "bad directive" error since we're just looking for
                                the term */
    }
    free(term_with_period);
    return result;
}

/* Return whether term is register */
bool is_register(char *term) {
    return word_in_array(term, REGISTER_COUNT, registers);
}

/* Return whether term is register used for indexing operations */
bool is_index_register(char *term) {
    return word_in_array(term, INDEX_REGISTER_COUNT, index_registers);
}

/* Return whether a word is found in an array */
bool word_in_array(char *term, int array_length, char *array[]) {
    int index;
    for (index = 0; index < array_length; index++) {
        if (match_word(term, array[index])) {
            return true;
        }
    }
    return false;
}

/* Return whether a term is reserved */
bool is_reserved(char *term) {
    return (is_instruction(term) || is_directive(term) || is_register(term));
}

/* Return whether addressing is legal for instruction and argument */
bool is_addressing_legal(int instruction, addressing to_check, int which_argument) {
    addressing relevant_addressing;

    /* Addressing marked as illegal */
    if (to_check == ADDRESSING_ILLEGAL) {
        return false;
    }

    if (which_argument == FIRST) {
        relevant_addressing = first_argument_addressing[instruction];
    } else {
        relevant_addressing = second_argument_addressing[instruction];
    }

    /* If an 'or' operation increases the total value, the addressing is illegal */
    if ((relevant_addressing | to_check) != relevant_addressing) {
        return false;
    }
    return true;
}

/* Return addressing method used in provided line */
addressing addressing_method(char *line) {
    addressing method;
    if (line == NULL || *line == '\0') {
        return ADDRESSING_NONE;
    } 
    if (is_addressing_immediate(line)) {
        method = ADDRESSING_IMMEDIATE;
    } else if (is_register(line)) {
        method = ADDRESSING_REGISTER;
    } else if (is_addressing_index(line)) {
        method = ADDRESSING_INDEX;
    } else if (is_addressing_direct(line)) {
        method = ADDRESSING_DIRECT;
    } else {
        method = ADDRESSING_ILLEGAL;
    }
    if (g_error != NO_ERRORS) {
        method = ADDRESSING_ILLEGAL;
    }
    return method;
}

/* Return whether addressing is immediate */
bool is_addressing_immediate(char *line) {
    /* Immediate addressing must start with '#' */
    if (*line != '#') {
        return false;
    }

    line++;
    /* Ignore value type specification */
    if (*line == '-' || *line == '+') {
        line++;
    }

    /* Ensure all content is digits until next argument or end of line */
    while (*line != ',' && *line != '\0' && !isspace(*line)) {
        if (!isdigit(*line)) {
            g_error = ERROR_ADDRESSING;
            return false;
        }
        line++;
    }
    return true;
}

/* Return whether addressing is index addressing */
bool is_addressing_index(char *line) {
    char potential_register[4] = "\0";
    char potential_label[MAX_LABEL_LENGTH] = "\0";
    word_in_brackets(line, potential_register);
    word_before_brackets(line, potential_label);

    /* If there's usage of a register */
    if (potential_register[0] != '\0') {
        /* Ensure legal usage */
        if (is_index_register(potential_register) && is_addressing_direct(potential_label)) {
            return true;
        }
        g_error = ERROR_ADDRESSING;
    }
    return false;
}

/* Return whether addressing is direct */
bool is_addressing_direct(char *line) {
    char *fake_label;
    char placeholder[MAX_LINE_LENGTH];

    /* Utilise check_for_label functionality to detemrine existence of label */
    fake_label = concatenate(line, ":");
    if (check_for_label(fake_label, placeholder)) {
        free(fake_label);
        return true;
    }

    /* Unset errors since we aggressively simulated label usage */
    g_error = NO_ERRORS;
    free(fake_label);
    return false;
}
