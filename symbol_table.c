#include "common.h"
#include "symbol_table.h"

symbol_list symbols = NULL;

void add_symbol(char *identifier, int value, int base_address, int offset, attribute *attributes) {
    struct symbol data;
    struct symbol_node *new_node = (struct symbol_node *) safe_malloc(sizeof(struct symbol_node));
    struct symbol_node *current_node;

    data.identifier = strdup(identifier);
    data.value = value;
    data.base_address = base_address;
    data.offset = offset;
    data.attributes = attributes;
    new_node->next = NULL;
    new_node->data = data;

    if (symbols == NULL) {
        symbols = new_node;
        return;
    }
    current_node = symbols;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
}

bool symbol_exists(char *identifier) {
    struct symbol_node *current_node = symbols;
    while (current_node != NULL) {
        if (strings_match(identifier, current_node->data.identifier)) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}

void clear_symbols() { /* This needs to be redone properly */
    struct symbol_node *current_node, *next_node;
    current_node = symbols;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}