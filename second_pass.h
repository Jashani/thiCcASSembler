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

/**
 * @brief perform second pass functionality on provided file,
 * then populate symbols according to new data.
 *
 * @param file to process
 * @return true if successful, false otherwise
 */
bool second_pass(FILE *file);

/**
 * @brief process provided file according to second pass definition
 *
 * @param file to process
 * @return true if successful, false otherwise
 */
bool pass_over_file(FILE *file);

/**
 * @brief process given line according to second pass definition.
 * Again because we did it once in the first pass!
 *
 * @param line to process
 * @param current_line number of current line
 * @return true if successful, false otherwise
 */
bool process_line_again(char *line, int current_line);

/**
 * @brief process line containing a directive
 * Again because we did it once in the first pass!
 *
 * @param line containing a directive
 * @param directive_type found in line
 * @return true if successful, false otherwise
 */
bool handle_directive_again(char *line, directive directive_type);

#endif