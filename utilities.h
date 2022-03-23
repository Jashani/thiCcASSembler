/* Utilities
 * Miscellaneous functions used across the program
 */ 

#include "common.h"
#include "errors.h"

#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * @brief concatenate two values, return a newly malloced (!) value
 * 
 * @param start of string
 * @param end of string
 * @return char* 
 */
char *concatenate(char *start, char *end);

/**
 * @brief extend a string with a value. Usage: x = extend(x, y);
 * 
 * @param current string
 * @param extra to append
 * @return char* 
 */
char *extend(char *current, char *extra);

/**
 * @brief malloc and check success
 * 
 * @param size of memory
 * @return void* 
 */
void *safe_malloc(long size);

/**
 * @brief return first word found in line based on whitespaces
 * 
 * @param line to check
 * @param result where the result will go
 */
void extract_first_word(char *line, char *result);

/**
 * @brief check whether the first word in a line matches a given word
 * 
 * @param line to check
 * @param word to match
 * @return true if match, false otherwise
 */
bool match_word(char *line, char *word);

/**
 * @brief move to next field in line
 * 
 * @param line to move in
 * @return char* new line
 */
char *next_field(char *line);

/**
 * @brief chop all spaces up to first word in line
 * 
 * @param line to chop
 * @return char* new line
 */
char *trim(char *line);

/**
 * @brief returns whether strings match exactly
 * 
 * @param first string
 * @param second string
 * @return true if match, false otherwise
 */
bool strings_match(char *first, char *second);

/**
 * @brief get word in square brackets. Input would be "[abc]"
 * 
 * @param line to check
 * @param word to set
 */
void word_in_brackets(char *line, char *word);

/**
 * @brief get word right before square brackets. Input would be "abc[...]..."
 * 
 * @param line to check
 * @param word to set
 */
void word_before_brackets(char *line, char *word);

/**
 * @brief returns value trimmed to 16 bits
 * 
 * @param value to trim
 * @return int result
 */
int set_to_size16(int value);

/**
 * @brief determine whether line should be processed or not
 * 
 * @param line to check
 * @param current_line number
 * @return true if should process, false otherwise
 */
bool should_process_line(char *line, int current_line);

/**
 * @brief determine whether line start is legal
 * 
 * @param line to check
 * @return true if legal, false otherwise
 */
bool valid_start(char *line);

#endif