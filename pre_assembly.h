#include <stdio.h>

/**
 * @brief Perform pre-assembly algorithm
 * 
 * @param original_file file to run pre-assembly on
 * @return bool success
 */
bool pre_assembly(FILE *original_file);

/**
 * @brief Replace macro call with macro content
 * 
 * @param file 
 * @return bool success 
 */
bool expand_macro(FILE *file, macro);

/**
 * @brief Delete the line currently read in the file
 * 
 * @param file 
 * @return bool success
 */
bool delete_current_line(FILE *file);