#include "common.h"
#include "globals.h"
#include "term_detection.h"
#include "utilities.h"
#include "symbol_table.h"

#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H

#define FUNCTOR_LOCATION 12
#define SOURCE_REGISTER_LOCATION 8
#define SOURCE_ADDRESSING_LOCATION 6
#define TARGET_REGISTER_LOCATION 2
#define TARGET_ADDRESSING_LOCATION 0

#define NO_VALUE 0

typedef enum encoding_type {
    ENCODING_ABSOLUTE = 1 << 18,
    ENCODING_RELOCATABLE = 1 << 17,
    ENCODING_EXTERNAL = 1 << 16
} encoding_type;

typedef struct binary_node {
    struct binary_node *next;
    int value;
    char *future_label; /* In case a node needs to be populated later */
} binary_node;

typedef binary_node* binary_list;

bool add_to_code_image(int data, char *future_label);
bool add_to_data_image(int data);
bool add_to_image(binary_list image, int data);
int addressing_to_value(addressing to_convert);
void print_data_image();
void print_code_image();
int build_binary_instruction(encoding_type encoding, int functor,
                             int target_addressing, int target_register,
                             int source_addressing, int source_register);
bool populate_symbols();

#endif