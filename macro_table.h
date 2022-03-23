/* Macro Table
 * Provides interactions with macros.
 */

#include "common.h"
#include "utilities.h"

#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macro {
    char *name;
    char *content;
} macro;

typedef struct macro_node {
    struct macro_node *next;
    macro data;
} macro_node;

typedef macro_node* macro_list;

/**
 * @brief add macro to macro table
 * 
 * @param name of macro
 * @param content of macro
 */
void add_macro(char *name, char *content);

/**
 * @brief check if given macro exists.
 *
 * @param name of macro
 * @return true if exists, false otherwise
 */
bool macro_exists(char *name);

/**
 * @brief return the specified macro
 * 
 * @param name of macro to return
 * @return char* macro
 */
char* get_macro(char *name);

/**
 * @brief free macros
 * 
 */
void clear_macros();
void print_macros();

#endif