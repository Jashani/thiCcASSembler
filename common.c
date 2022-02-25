#include "common.h"

char* concatenate(char *start, char *end) {
    const int NULL_INDEX = 1;
    char *result = (char *) safe_malloc(strlen(start) + strlen(end) + NULL_INDEX);
    strcpy(result, start);
    strcat(result, end);
    return result;
}

void *safe_malloc(long size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        printf("Failed to allocate memory, exiting.");
        exit(FAILURE);
    }
    return pointer;
}