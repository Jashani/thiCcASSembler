#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macro {
    char *name;
    char *content;
} macro;

typedef struct macro_node {
    struct macro_node *next;
    macro data;
} macro_node;

typedef macro_node* macro_list;

void add_macro(char *name, char *content);
bool macro_exists(char *name);
char* get_macro(char *name);
void clear_macros();
void print_macros();

#endif