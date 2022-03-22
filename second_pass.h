/* Second Pass
 * Perform second pass functionality of file.
 * Recurring functions will have the suffix '_again' hehe */

#include "common.h"
#include "errors.h"
#include "globals.h"
#include "symbol_table.h"
#include "machine_code.h"

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

bool second_pass(FILE *file);
bool pass_over_file(FILE *file);
bool process_line_again(char *line, int current_line);
bool handle_directive_again(char *line, directive directive_type);

#endif