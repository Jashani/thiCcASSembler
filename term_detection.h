#include "common.h"
#include "errors.h"
#include "globals.h"
#include "symbol_table.h"

#ifndef TERM_DETECTION_H
#define TERM_DETECTION_H

#define INSTRUCTION_COUNT 16
#define REGISTER_COUNT 8

bool check_for_directive(char *line, directive *directive_to_set);
bool check_for_instruction(char *line, int *instruction_index);
int instruction_opcode(int index);

bool is_instruction(char *term);
bool is_directive(char *term);
bool is_register(char *term);
bool is_reserved(char *term);

#endif