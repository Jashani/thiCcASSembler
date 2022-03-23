/* Machine Code
 * Provides access to code and data images,
 * for populating images and interacting with them.
 */

#include "common.h"
#include "globals.h"
#include "term_detection.h"
#include "utilities.h"
#include "symbol_table.h"

#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H

/* Bit locations of values in binary data */
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

/**
 * @brief add data to code image. Potentially attach label to populate in the
 * future.
 *
 * @param data to add
 * @param future_label if attached to instruction
 * @return true if success, false if failure
 */
bool add_to_code_image(int data, char *future_label);

/**
 * @brief add data to data image.
 *
 * @param data to add
 * @return true if success, false if failure
 */
bool add_to_data_image(int data);

/**
 * @brief Convert type of addressing to value used in binary instruction.
 * 
 * @param to_convert the addressing type
 * @return real value
 */
int addressing_to_value(addressing to_convert);
void print_data_image();
void print_code_image();

/**
 * @brief build the data to add to code image.
 *
 * @param encoding type
 * @param functor if applicable to instruction
 * @param target_addressing addressing of target argument
 * @param target_register
 * @param source_addressing addressing of source argument
 * @param source_register
 * @return the binary instruction
 */
int build_binary_instruction(encoding_type encoding, int functor,
                             int target_addressing, int target_register,
                             int source_addressing, int source_register);

/**
 * @brief populate symbols marked earlier with future_label,
 * using data from symbol table.
 *
 * @return true if success, false if failure
 */
bool populate_symbols();

/**
 * @brief legibly write content of images to file
 *
 * @param file to write to
 */
void write_image_output(FILE *file);

/**
 * @brief write binary line converted to special obj base (Au-Bv-Cw-Dx-Ey)
 *
 * @param file to write to
 */
void write_binary_string(FILE *file, int data, int line);

/**
 * @brief write external symbols to file using image data
 * 
 * @param file to write to
 */
void write_external_symbols(FILE *file);

/**
 * @brief free memory of given image
 * 
 * @param image to free
 */
void clear_image(binary_list image);

/**
 * @brief free memory of both data images
 * 
 */
void clear_images();

#endif