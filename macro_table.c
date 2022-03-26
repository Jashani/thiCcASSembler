#include "macro_table.h"

macro_list macros = NULL;

/* Add macro to macro table */
void add_macro(char *name, char *content) {
    struct macro data;
    struct macro_node *new_node = (struct macro_node *) safe_malloc(sizeof(struct macro_node));
    struct macro_node *current_node;

    /* We need to strdup to make sure data is saved and unchangeable */
    data.name = strdup(name);
    data.content = strdup(content);
    new_node->next = NULL;
    new_node->data = data;

    /* If we haven't created the macros table yet, set it to this macro */
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

/* Return whether macro exists */
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

/* Return macro content - should be used after macro_exists */
char* get_macro(char *name) {
    struct macro_node *current_node = macros;
    while (current_node != NULL) {
        if (strings_match(name, current_node->data.name)) {
            return current_node->data.content;
        }
        current_node = current_node->next;
    }
    return NULL;
}

/* Free macro table */
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
    macros = NULL;
}