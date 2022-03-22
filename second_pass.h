#include "common.h"
#include "errors.h"
#include "globals.h"
#include "symbol_table.h"
#include "machine_code.h"

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

bool second_pass(FILE *file);
bool process_line(char *line, int current_line);
bool handle_directive(char *line, directive directive_type);

#endif