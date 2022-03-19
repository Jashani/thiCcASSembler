#include "common.h"
#include "macro_table.h"

macro_list macros = NULL;

void add_macro(char *name, char *content) {
    print_macros();
    struct macro data;
    struct macro_node *new_node = (struct macro_node *) safe_malloc(sizeof(struct macro_node));
    struct macro_node *current_node;

    data.name = strdup(name);
    data.content = strdup(content);
    new_node->next = NULL;
    new_node->data = data;

    if (macros == NULL) {
        macros = new_node;
        return;
    }
    current_node = macros;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
}

bool macro_exists(char *name) {
    struct macro_node *current_node = macros;
    while (current_node != NULL) {
        if (strings_match(name, current_node->data.name)) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}

char* get_macro(char *name) {
    struct macro_node *current_node = macros;
    while (current_node != NULL) {
        if (strings_match(name, current_node->data.name)) {
            return current_node->data.content;
        }
        current_node = current_node->next;
    }
    return false;
}

void print_macros() { /* Debug method */
    struct macro_node *current_node;
    current_node = macros;
    while (current_node != NULL) {
        current_node = current_node->next;
    }
}

void clear_macros() {
    struct macro_node *current_node, *next_node;
    current_node = macros;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node->data.name);
        free(current_node->data.content);
        free(current_node);
        current_node = next_node;
    }
}