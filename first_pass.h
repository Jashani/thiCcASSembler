#include <stdio.h>

bool first_pass(FILE *file, int *instruction_counter, int *data_counter);
bool handle_data_line(FILE *file, int *data_counter);
bool handle_external_line(FILE *file);
bool handle_instruction_line(FILE *file, int *instruction_counter);
bool recalculate_symbol_values();