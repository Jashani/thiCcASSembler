#include "common.h"
#include "symbol_table.h"

symbol_list symbols = NULL;

bool add_symbol(char *identifier, int value, attribute_set attributes) {
    struct symbol data;
    struct symbol_node *new_node, *current_node;

    if (symbol_exists(identifier)) {
        g_error = ERROR_SYMBOL_EXISTS;
        return false;
    }

    new_node = (struct symbol_node *)safe_malloc(sizeof(struct symbol_node));
    data.identifier = strdup(identifier);
    data.value = value;
    data.base_address = value - (value % ADDRESS_INTERVAL);
    data.offset = value % ADDRESS_INTERVAL;
    data.attributes = attributes;
    new_node->next = NULL;
    new_node->data = data;

    if (symbols == NULL) {
        symbols = new_node;
        return true;
    }
    current_node = symbols;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
    return true;
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

bool finalise_data() {
    int new_value;
    struct symbol_node *current_node;
    current_node = symbols;
    while (current_node != NULL) {
        if ((current_node->data.attributes | ATTRIBUTE_DATA) == ATTRIBUTE_DATA) {
            new_value = current_node->data.value + g_instruction_counter;
            current_node->data.base_address = new_value - (new_value % ADDRESS_INTERVAL);
            current_node->data.offset = (new_value % ADDRESS_INTERVAL);
            current_node->data.value = new_value;
        }
        current_node = current_node->next;
    }
}

void print_symbols() { /* Debug method */
    struct symbol_node *current_node;
    current_node = symbols;
    printf("Printing symbols.\n\n");
    while (current_node != NULL) {
        printf("Symbol %s:\nValue: %d, Base: %d, Offset: %d, Attributes: %d\n",
               current_node->data.identifier,
               current_node->data.value,
               current_node->data.base_address,
               current_node->data.offset,
               current_node->data.attributes);
        current_node = current_node->next;
    }
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