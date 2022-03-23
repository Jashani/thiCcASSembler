/* First pass
 * Perform the first pass on a given file.
 * Populates code and data images for each line, according to line content.
 */

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

/**
 * @brief perform first pass functionality on provided file
 *
 * @param file file to process
 * @return true if success, false if failure
 */
bool first_pass(FILE *file);

/**
 * @brief processes a single line based on first pass norms
 *
 * @param line content of line
 * @param current_line
 * @return true if success, false if failure
 */
bool process_line(char *line, int current_line);

/**
 * @brief ensure line structure is proper and arguments provided suitably
 *
 * @param line to check
 * @param arguments count of arguments for instruction
 * @param first_argument
 * @param second_argument
 * @return true if success, false if failure
 */
bool valid_line(char *line, int arguments, char *first_argument, char *second_argument);

/**
 * @brief split line into two arguments following expected argument structure
 * 
 * @param line to split
 * @param first argument to fill
 * @param second argument to fill
 */
void break_arguments(char *line, char *first, char *second);

/**
 * @brief process directive as per first pass norms
 *
 * @param line to process
 * @param directive_type found in line
 * @param label if exists in line
 * @return true if success, false if failure
 */
bool handle_directive(char *line, directive directive_type, char *label);

/**
 * @brief process instruction as per first pass norms
 *
 * @param line to process
 * @param instruction found in line, associated with term_detection
 * @param label if exists in line
 * @return true if success, false if failure
 */
bool handle_instruction(char *line, int instruction, char *label);

/**
 * @brief encode instruction into code images as per image structure definition
 *
 * @param arguments count of arguments in line
 * @param instruction found in line, associated with term_detection
 * @param first_argument
 * @param second_argument
 * @return true if success, false if failure
 */
bool encode_instruction(int arguments, int instruction, char *first_argument,
                        char *second_argument);

/**
 * @brief add data to code image based on addressing used in line
 *
 * @param addressing_type
 * @param argument to add data for
 * @param argument_slot first or second argument in line
 * @return true if success, false if failure
 */
bool add_addressing_data(addressing addressing_type, char *argument, int argument_slot);

/**
 * @brief extract value from argument using immediate addressing (#-16 -> -16)
 * 
 * @param argument to extract value from
 * @return int 
 */
int extract_immediate_value(char *argument);

/**
 * @brief handle directive of type data
 *
 * @param line containing directive
 * @return true if success, false if failure
 */
bool handle_directive_data(char *line);

/**
 * @brief handle directive of type string
 *
 * @param line containing directive
 * @return true if success, false if failure
 */
bool handle_directive_string(char *line);

/**
 * @brief handle directive of type extern
 *
 * @param line containing directive
 * @return true if success, false if failure
 */
bool handle_directive_extern(char *line);

#endif