#include "common.h"
#include "macro_table.h"

macro_list macros = NULL;

void add_macro(char *name, char **content) {
    struct macro data;
    struct macro_node *new_node = (struct macro_node *)malloc(sizeof(struct macro_node));
    struct macro_node *current_node;

    data.name = name;
    data.content = content;
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
        if (strcmp(name, current_node->data.name) == 0) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}

char** get_macro(char *name) {
    struct macro_node *current_node = macros;
    while (current_node != NULL) {
        if (strcmp(name, current_node->data.name) == 0) {
            return current_node->data.content;
        }
        current_node = current_node->next;
    }
    return false;
}
