#include "addressing.h"
#include "common.h"
#include "errors.h"
#include "globals.h"
#include "machine_code.h"
#include "macro_table.h"
#include "symbol_table.h"
#include "term_detection.h"
#include "utilities.h"

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

bool first_pass(FILE *file);
bool process_line(char *line, int current_line);

bool valid_line(char *line, int arguments, char *first_argument, char *second_argument);
void break_arguments(char *line, char *first, char *second);
bool handle_directive(char *line, directive directive_type, char *label);
bool handle_instruction(char *line, int instruction_index, char *label);
bool encode_instruction(int arguments, int instruction, char *first_argument,
                        char *second_argument);
bool add_addressing_data(addressing addressing_type, char *argument, int argument_slot);

int extract_immediate_value(char *argument);

bool handle_directive_data(char *line);
bool handle_directive_string(char *line);
bool handle_directive_extern(char *line);

#endif