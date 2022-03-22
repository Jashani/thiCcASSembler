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

bool first_pass(FILE *file);
bool should_process_line(char *line, int current_line);
bool process_line(char *line, int current_line);
bool check_for_label(char *line, char *label);

bool valid_line(char *line, int arguments, char *first_argument, char *second_argument);
void break_arguments(char *line, char *first, char *second);
bool handle_directive(char *line, directive directive_type, char *label);
bool handle_instruction(char *line, int instruction_index, char *label);
bool encode_instruction(int arguments, int instruction, char *first_argument,
                        char *second_argument);

bool add_addressing_data(addressing addressing_type, char *argument, int argument_slot);
addressing addressing_method(char *line);
bool is_addressing_immediate(char *line);
bool is_addressing_direct(char *line);
bool is_addressing_index(char *line);

int extract_immediate_value(char *argument);
int set_to_base16(int value);

bool handle_directive_data(char *line);
bool handle_directive_string(char *line);
bool handle_directive_extern(char *line);

#endif