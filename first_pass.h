#include <stdio.h>
#include "globals.h"
#include "common.h"
#include "errors.h"
#include "symbol_table.h"
#include "macro_table.h"
#include "term_detection.h"

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

bool first_pass(FILE *file);
bool should_process_line(char *line, int current_line);
bool process_line(char *line, int current_line);
bool check_for_label(char *line, char *label);
bool handle_instruction(char *line, int instruction_index, char *label);
bool handle_directive(char *line, directive directive_type, char *label);

bool handle_directive_data(char *line);
bool handle_directive_string(char *line);

#endif