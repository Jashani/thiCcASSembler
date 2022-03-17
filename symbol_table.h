
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
    unsigned int base_address;
    unsigned int offset;
    attribute *attributes;
} symbol;

typedef struct symbol_node {
    symbol_node *next;
    symbol data;
} symbol_node;

typedef symbol_node* symbol_list;