
typedef struct macro {
    char *name;
    char **content;
} macro;

typedef struct macro_node {
    macro_node *next;
    macro data;
} macro_node;

typedef macro_node* macro_list;

