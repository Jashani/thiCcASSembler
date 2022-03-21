#include <stdio.h>
#include "globals.h"
#include "common.h"
#include "errors.h"
#include "symbol_table.h"
#include "macro_table.h"
#include "term_detection.h"
#include "machine_code.h"

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

typedef enum addressing {
    ADDRESSING_NONE,
    ADDRESSING_IMMEDIATE,
    ADDRESSING_DIRECT,
    ADDRESSING_INDEX,
    ADDRESSING_REGISTER,
    ADDRESSING_ILLEGAL
} addressing;

bool first_pass(FILE *file);
bool should_process_line(char *line, int current_line);
bool process_line(char *line, int current_line);
bool check_for_label(char *line, char *label);
bool handle_instruction(char *line, int instruction_index, char *label);
bool handle_directive(char *line, directive directive_type, char *label);

bool handle_directive_data(char *line);
bool handle_directive_string(char *line);
bool handle_directive_extern(char *line);

#endif