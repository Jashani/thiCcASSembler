#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 81
#define SUCCESS 0
#define FAILURE -1
#define false 0
#define true 1
typedef int bool;

char* concatenate(char *start, char *end);
void *safe_malloc(long size);