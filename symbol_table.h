#include "errors.h"
#include "globals.h"

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
    ATTRIBUTE_CODE = 1,
    ATTRIBUTE_ENTRY = 2,
    ATTRIBUTE_DATA = 4,
    ATTRIBUTE_EXTERNAL = 8
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
bool symbol_exists(char *identifier);
bool finalise_data();
void clear_symbols();
void print_symbols();

#endif