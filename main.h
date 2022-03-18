#include <stdio.h>
#include "common.h"
#include "pre_assembly.h"
#include "first_pass.h"

#ifndef MAIN_H
#define MAIN_H

int main(int argc, char *argv[]);

/**
 * @brief Build paths to process from commandline arguments
 * 
 * @param path_count
 * @param file_names names of files without suffix 
 * @param paths a pointer which will be populated with the files names
 */
void assemblify_paths(int path_count, char *file_names[], char *paths[]);

/**
 * @brief Run full assembler process on file
 * 
 * @param path path to file
 * @return bool success value
 */
bool process_file(char *path);

bool create_output_files();

#endif
