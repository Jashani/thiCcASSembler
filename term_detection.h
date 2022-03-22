/* Term Detection
 * Responsible for identifying terms and providing
 * Additional information about them */

#include "common.h"
#include "errors.h"
#include "globals.h"
#include "symbol_table.h"

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

bool check_for_directive(char *line, directive *directive_to_set);
bool check_for_instruction(char *line, int *instruction_index);
int instruction_opcode(int index);
int instruction_functor(int index);
int instruction_arguments(int index);
int register_to_value(char *register_name);

bool is_addressing_legal(int instruction, addressing to_check, int which_argument);

bool is_instruction(char *term);
bool is_directive(char *term);
bool is_register(char *term);
bool is_index_register(char *term);
bool is_reserved(char *term);

bool word_in_array(char *term, int array_length, char *array[]);

#endif