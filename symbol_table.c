#include "symbol_table.h"

symbol_list symbols = NULL;

/* Add symbol to symbol table */
bool add_symbol(char *identifier, int value, attribute_set attributes) {
    struct symbol data;
    struct symbol_node *new_node, *current_node;

    /* Ensure unique */
    if (symbol_exists(identifier)) {
        g_error = ERROR_SYMBOL_EXISTS;
        return false;
    }

    new_node = (struct symbol_node *)safe_malloc(sizeof(struct symbol_node));
    data.identifier = strdup(identifier);
    data.value = value;
    base_and_offset(value, &data.base_address, &data.offset);
    data.attributes = attributes;
    new_node->next = NULL;
    new_node->data = data;

    /* If we haven't created the table yet, set it to this node. */
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

/* Add a new attribute to an existing symbol */
bool add_attribute_to_symbol(char *identifier, attribute new_attribute) {
    struct symbol_node *current_node;
    current_node = get_symbol(identifier);
    if (current_node == NULL) {
        g_error = ERROR_NO_SYMBOL;
        return false;
    }
    current_node->data.attributes |= new_attribute;
    return true;
}

/* Return whether symbol exists */
bool symbol_exists(char *identifier) {
    return (get_symbol(identifier) != NULL);
}

/* Set symbol data to given arguments, according to identifier */
bool symbol_data(char *identifier, int *base, int *offset,
                 attribute_set *attributes) {
    struct symbol_node *current_node;
    current_node = get_symbol(identifier);
    if (current_node == NULL) {
        g_error = ERROR_NO_SYMBOL;
        return false;
    }

    *base = current_node->data.base_address;
    *offset = current_node->data.offset;
    *attributes = current_node->data.attributes;
    return true;
}

/* Return specified symbol */
symbol_node *get_symbol(char *identifier) {
    struct symbol_node *current_node = symbols;
    while (current_node != NULL) {
        if (match_word(identifier, current_node->data.identifier)) {
            return current_node;
        }
        current_node = current_node->next;
    }
    return NULL;
}

/* Set data entry values according to instruction cout */
void finalise_data() {
    int new_value;
    struct symbol_node *current_node;
    current_node = symbols;
    while (current_node != NULL) {
        if ((current_node->data.attributes | ATTRIBUTE_DATA) == ATTRIBUTE_DATA) {
            new_value = current_node->data.value + g_instruction_counter;
            base_and_offset(new_value, &current_node->data.base_address,
                            &current_node->data.offset);
            current_node->data.value = new_value;
        }
        current_node = current_node->next;
    }
}

/* Free symbol data */
void clear_symbols() {
    struct symbol_node *current_node, *next_node;
    current_node = symbols;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node->data.identifier);
        free(current_node);
        current_node = next_node;
    }
    symbols = NULL;
}

/* Return base and offset values calculated according to provided address */
void base_and_offset(int address, int *base, int *offset) {
    *base = address - (address % ADDRESS_INTERVAL);
    *offset = address % ADDRESS_INTERVAL;
}

/* Return whether specified attribute exists on any symbol */
bool attribute_symbol_exists(attribute to_find) {
    struct symbol_node *current_node;
    current_node = symbols;
    while (current_node != NULL) {
        if ((current_node->data.attributes & to_find) == to_find) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}

/* Return whether any symbol is marked as entry */
bool entry_symbols_exist() {
    return attribute_symbol_exists(ATTRIBUTE_ENTRY);
}

/* Export entry symbols data to file */
void write_entry_symbols(FILE *file) {
    struct symbol_node *current_node;
    current_node = symbols;
    while (current_node != NULL) {
        if ((current_node->data.attributes & ATTRIBUTE_ENTRY) == ATTRIBUTE_ENTRY) {
            fprintf(file, "%s,%d,%d\n", current_node->data.identifier,
                    current_node->data.base_address, current_node->data.offset);
        }
        current_node = current_node->next;
    }
}

/* Return whether any symbol is marked as external */
bool external_symbols_exist() {
    return attribute_symbol_exists(ATTRIBUTE_EXTERNAL);
}
