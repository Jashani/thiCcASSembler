#include "term_detection.h"

#ifndef CONSTANT_ARRAYS
#define CONSTANT_ARRAYS

char *instructions[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                        "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

int instruction_opcodes[] = {0, 1, 2, 2, 4, 5, 5, 5, 5, 9, 9, 9, 12, 13, 14, 15};
int instruction_functors[] = {0, 0, 10, 11, 0, 10, 11, 12, 13, 10, 11, 12, 0, 0, 0, 0};

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

char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

#endif

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

    g_error = ERROR_BAD_DIRECTIVE;
    return false;
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

int register_to_value(char *register_name) {
    int index;
    for (index = 0; index < REGISTER_COUNT; index++) {
        if (match_word(register_name, registers[index])) {
            return index;
        }
    }
    return false;
}

int instruction_opcode(int index) {
    return instruction_opcodes[index];
}

int instruction_functor(int index) {
    return instruction_functors[index];
}

int instruction_arguments(int index) {
    if (index <= 4) {
        return 2;
    } else if (index >= 12) {
        return 0;
    } else {
        return 1;
    }
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
        g_error = NO_ERRORS; /* Unset bad directive since we're just looking for
                                the term */
    }
    free(term_with_period);
    return result;
}

bool is_register(char *term) {
    int index;
    for (index = 0; index < REGISTER_COUNT; index++) {
        if (match_word(term, registers[index])) {
            return true;
        }
    }
    return false;
}

bool is_reserved(char *term) {
    return (is_instruction(term) || is_directive(term) || is_register(term));
}

bool is_addressing_legal(int instruction, addressing to_check, int which_argument) {
    addressing relevant_addressing;
    if (which_argument == FIRST) {
        relevant_addressing = first_argument_addressing[instruction];
    } else {
        relevant_addressing = second_argument_addressing[instruction];
    }

    /* If an or operation increases the total value, the addressing is illegal */
    if ((relevant_addressing | to_check) != relevant_addressing) {
        return false;
    }
    return true;
}
