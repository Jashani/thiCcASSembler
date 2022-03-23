#include "common.h"
#include "errors.h"
#include "macro_table.h"
#include "term_detection.h"
#include "utilities.h"

#ifndef PRE_ASSEMBLY_H
#define PRE_ASSEMBLY_H

/**
 * @brief Perform pre-assembly algorithm
 * 
 * @param original_file file to run pre-assembly on
 * @return bool success
 */
bool pre_assembly(FILE *source_file, FILE *expanded_source_file);

#endif
