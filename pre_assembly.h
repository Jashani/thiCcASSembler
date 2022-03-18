#include <stdio.h>
#include "common.h"
#include "macro_table.h"

#ifndef PRE_ASSEMBLY_H
#define PRE_ASSEMBLY_H

/**
 * @brief Perform pre-assembly algorithm
 * 
 * @param original_file file to run pre-assembly on
 * @return bool success
 */
bool pre_assembly(FILE *original_file);

/**
 * @brief Delete the line currently read in the file
 * 
 * @param file 
 * @return bool success
 */
bool delete_current_line(FILE *file);

bool extract_first_word(char *line, char *result);

#endif