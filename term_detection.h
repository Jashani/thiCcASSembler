/* Term Detection
 * Responsible for identifying terms and providing
 * Additional information about them */

#include "common.h"
#include "errors.h"
#include "globals.h"
#include "symbol_table.h"
#include "utilities.h"

#ifndef TERM_DETECTION_H
#define TERM_DETECTION_H

#define INSTRUCTION_COUNT 16
#define REGISTER_COUNT 16
#define INDEX_REGISTER_COUNT 6

#define FIRST 1
#define SECOND 2

typedef enum addressing {
    ADDRESSING_NONE = 0,
    ADDRESSING_IMMEDIATE = 1 << 0,
    ADDRESSING_DIRECT = 1 << 1,
    ADDRESSING_INDEX = 1 << 2,
    ADDRESSING_REGISTER = 1 << 3,
    ADDRESSING_ILLEGAL = -1,
    ADDRESSING_ALL = ADDRESSING_IMMEDIATE | ADDRESSING_DIRECT | ADDRESSING_INDEX | ADDRESSING_REGISTER,
    ADDRESSING_DEFERRED = ADDRESSING_DIRECT | ADDRESSING_INDEX | ADDRESSING_REGISTER,
    ADDRESSING_VARIABLE = ADDRESSING_DIRECT | ADDRESSING_INDEX
} addressing;

/**
 * @brief check if directive is found in line and set accordingly
 *
 * @param line to check
 * @param directive_to_set if exists
 * @return true if exists, false otherwise
 */
bool check_for_directive(char *line, directive *directive_to_set);

/**
 * @brief check if instruction is found in line and set accordingly
 *
 * @param line to check
 * @param instruction_index if exists
 * @return true if exists, false otherwise
 */
bool check_for_instruction(char *line, int *instruction_index);

/**
 * @brief check if label is found in line and set accordingly
 *
 * @param line to check
 * @param label if exists
 * @return true if exists, false otherwise
 */
bool check_for_label(char *line, char *label);

/**
 * @brief return opcode of instruction
 * 
 * @param index of instruction
 * @return int opcode
 */
int instruction_opcode(int index);

/**
 * @brief return functor value of instruction
 *
 * @param index of instruction
 * @return int functor value
 */
int instruction_functor(int index);

/**
 * @brief return arguments count of instruction
 *
 * @param index of instruction
 * @return int argument count
 */
int instruction_arguments(int index);

/**
 * @brief convert name to its value (r8 -> 8)
 *
 * @param register_name to convert
 * @return int value of register
 */
int register_to_value(char *register_name);

/**
 * @brief determine whether addressing is legal for given instruction
 * and argument position (first or second).
 *
 * @param instruction to check
 * @param to_check addressing to check
 * @param which_argument position of argument (first or second)
 * @return true if legal, false otherwise
 */
bool is_addressing_legal(int instruction, addressing to_check, int which_argument);

/* Functions to determine whether a term is of a particular type */
bool is_instruction(char *term);
bool is_directive(char *term);
bool is_register(char *term);
/* A register which can be used for indexing (10-15) */
bool is_index_register(char *term);
bool is_reserved(char *term);

/**
 * @brief check if word is in array
 * 
 * @param term to find
 * @param array_length of array 
 * @param array to check
 * @return true if exists, false otherwise
 */
bool word_in_array(char *term, int array_length, char *array[]);

/**
 * @brief addressing method found in provided line
 * 
 * @param line to check
 * @return addressing method
 */
addressing addressing_method(char *line);

/* Check whether addressing in line is of a particular type */
bool is_addressing_immediate(char *line);
bool is_addressing_direct(char *line);
bool is_addressing_index(char *line);

#endif