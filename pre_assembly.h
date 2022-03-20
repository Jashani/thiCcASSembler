#include <stdio.h>
#include "common.h"
#include "errors.h"
#include "macro_table.h"
#include "term_detection.h"

#ifndef PRE_ASSEMBLY_H
#define PRE_ASSEMBLY_H

/**
 * @brief Perform pre-assembly algorithm
 * 
 * @param original_file file to run pre-assembly on
 * @return bool success
 */
bool pre_assembly(FILE *source_file, FILE *expanded_source_file);

/**
 * @brief Delete the line currently read in the file
 * 
 * @param file 
 * @return bool success
 */
bool delete_current_line(FILE *file);

#endif