#include <stdio.h>

bool second_pass(FILE *file, int *instruction_counter, int *data_counter);
bool handle_entry_direction(FILE *file);
bool complete_binary_encoding(FILE *file);
bool handle_external_symbol();