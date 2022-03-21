#include "term_detection.h"

#ifndef CONSTANT_ARRAYS
#define CONSTANT_ARRAYS

char *instructions[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "stop"};

int instruction_opcodes[] = {0, 1, 2, 2, 4, 5, 5, 5, 5, 9, 9, 9, 12, 13, 14, 15};

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

int instruction_opcode(int index) {
    return instruction_opcodes[index];
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