#include <stdio.h>
#include "globals.h"
#include "common.h"
#include "errors.h"
#include "symbol_table.h"

#define INSTRUCTION_COUNT 16

char *instructions[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

bool first_pass(FILE *file);
bool should_process_line(char *line, int current_line);
bool handle_data_line(FILE *file, int *data_counter);
bool handle_external_line(FILE *file);
bool handle_instruction_line(FILE *file, int *instruction_counter);
bool recalculate_symbol_values();