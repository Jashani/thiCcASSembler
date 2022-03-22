#include "common.h"
#include "errors.h"
#include "globals.h"
#include "utilities.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

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

bool add_symbol(char *identifier, int value, attribute_set attributes);
bool add_attribute_to_symbol(char *identifier, attribute new_attribute);
bool symbol_address(char *identifier, int *base, int *offset);
symbol_node *get_symbol(char *identifier);
bool symbol_exists(char *identifier);
bool finalise_data();
void clear_symbols();
void print_symbols();
void base_and_offset(int address, int *base, int *offset);

#endif