
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum directive {
    DIRECTIVE_DATA,
    DIRECTIVE_STRUCT,
    DIRECTIVE_STRING,
    DIRECTIVE_ENTRY,
    DIRECTIVE_EXTERNAL
} directive;

typedef enum attribute {
    ATTRIBUTE_CODE,
    ATTRIBUTE_ENTRY,
    ATTRIBUTE_DATA,
    ATTRIBUTE_EXTERNAL
} attribute;

typedef struct symbol {
    char *identifier;
    int value;
    int base_address;
    int offset;
    attribute *attributes;
} symbol;

typedef struct symbol_node {
    struct symbol_node *next;
    symbol data;
} symbol_node;

typedef symbol_node* symbol_list;

void add_symbol(char *identifier, int value, int base_address, int offset, attribute *attributes);
bool symbol_exists(char *identifier);
void clear_symbols();

#endif