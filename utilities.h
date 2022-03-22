#include "common.h"
#include "errors.h"

#ifndef UTILITIES_H
#define UTILITIES_H

char *concatenate(char *start, char *end);
void *safe_malloc(long size);
void extract_first_word(char *line, char *result);
bool match_word(char *line, char *word);
char *next_field(char *line);
char *trim(char *line);
bool strings_match(char *first, char *second);
void word_in_brackets(char *line, char *word);
void word_before_brackets(char *line, char *word);
int set_to_base16(int value);
bool should_process_line(char *line, int current_line);
bool valid_start(char *line);

#endif