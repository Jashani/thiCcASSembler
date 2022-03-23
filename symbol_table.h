#include "common.h"
#include "errors.h"
#include "globals.h"
#include "utilities.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* The interval between memory addresses */
#define ADDRESS_INTERVAL 16

typedef enum directive {
    DIRECTIVE_DATA,
    DIRECTIVE_STRING,
    DIRECTIVE_ENTRY,
    DIRECTIVE_EXTERNAL
} directive;

typedef enum attribute {
    ATTRIBUTE_NONE = 0,
    ATTRIBUTE_CODE = 1 << 0,
    ATTRIBUTE_ENTRY = 1 << 1,
    ATTRIBUTE_DATA = 1 << 2,
    ATTRIBUTE_EXTERNAL = 1 << 3
} attribute;

typedef int attribute_set;

typedef struct symbol {
    char *identifier;
    int value;
    int base_address;
    int offset;
    attribute_set attributes;
} symbol;

typedef struct symbol_node {
    struct symbol_node *next;
    symbol data;
} symbol_node;

typedef symbol_node* symbol_list;

/**
 * @brief convert provided data to symbol, add it to symbol table.
 * 
 * @param identifier of symbol
 * @param value symbol address
 * @param attributes 
 * @return true if success, false otherwise
 */
bool add_symbol(char *identifier, int value, attribute_set attributes);

/**
 * @brief add attribute to existing symbol in table
 *
 * @param identifier to add attribute to
 * @param new_attribute to add
 * @return true if success, false otherwise
 */
bool add_attribute_to_symbol(char *identifier, attribute new_attribute);

/**
 * @brief returns data for specified symbol
 *
 * @param identifier to retrieve
 * @param base to set according to symbol
 * @param offset to set
 * @param attributes to set
 * @return true if success, false otherwise
 */
bool symbol_data(char *identifier, int *base, int *offset, attribute_set *attributes);

/**
 * @brief return symbol according to identifier
 * 
 * @param identifier of symbol to return
 * @return symbol_node* 
 */
symbol_node *get_symbol(char *identifier);

/**
 * @brief check whether symbol exists or not
 *
 * @param identifier to check
 * @return true if success, false otherwise
 */
bool symbol_exists(char *identifier);

/**
 * @brief update values for data image, as they're unknown until
 * code has been processed
 *
 * @return true if success, false otherwise
 */
bool finalise_data();

/**
 * @brief free symbol data
 * 
 */
void clear_symbols();
void print_symbols();

/**
 * @brief return base address and offset according to given address
 * 
 * @param address to parse
 * @param base to set
 * @param offset to set
 */
void base_and_offset(int address, int *base, int *offset);

/**
 * @brief returns whether a symbol exists with the given attribute
 * 
 * @param to_find attribute to check the existence of
 * @return true if exists, false otherwise
 */
bool attribute_symbol_exists(attribute to_find);

/**
 * @brief returns whether a symbol with an entry attribute exists
 *
 * @return true if exists, false otherwise
 */
bool entry_symbols_exist();

/**
 * @brief legibly export entry symbols data to file
 * 
 * @param file to export data to
 */
void write_entry_symbols(FILE *file);

/**
 * @brief returns whether a symbol with an external attribute exists
 *
 * @return true if exists, false otherwise
 */
bool external_symbols_exist();

#endif