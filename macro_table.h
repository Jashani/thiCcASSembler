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

#endif