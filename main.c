#include "main.h"

void assemblify_paths(int path_count, char *file_names[], char *paths[]) {
    int index;
    for (index = 0; index < path_count; index++) {
        paths[index] = concatenate(file_names[index], ".as");
    }
}

bool process_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Failed to open file: %s. Skipping.\n", path);
        free(path);
        return false;
    }

    first_pass(file);

    free(path);
    return true;
}

int main(int argc, char *argv[]) {
    int index;
    int path_count = argc - 1;
    char **file_names = argv + 1;
    char **paths = (char **) safe_malloc(path_count * sizeof(char *));

    assemblify_paths(path_count, file_names, paths);
    for (index = 0; index < path_count; index++) {
        process_file(paths[index]);
    }

    free(paths);
    return SUCCESS;
}