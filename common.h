#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef COMMON_H
#define COMMON_H

#define MAX_LINE_LENGTH 80 + 1 + 1  /* Line length + \n + \0 */
#define MAX_LABEL_LENGTH 31
#define SUCCESS 0
#define FAILURE -1
#define false 0
#define true 1
typedef int bool;

char* concatenate(char *start, char *end);
void* safe_malloc(long size);
void extract_first_word(char *line, char *result);
bool match_word(char *line, char *word);
char *next_field(char *line);
char *trim(char *line);
bool strings_match(char *first, char *second);
void word_in_brackets(char *line, char *word);
void word_before_brackets(char *line, char *word);
int set_to_base16(int value);

#endif
