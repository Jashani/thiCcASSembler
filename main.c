#include "main.h"

/* using without free causes mem leak */
char *assembly_path(char *file_name) { 
    return concatenate(file_name, ".as");
}

/* using without free causes mem leak */
char *expanded_assembly_path(char *file_name) {
    return concatenate(file_name, ".am");
}

bool process_file(char *path) {
    bool success;
    FILE *source_file, *expanded_source_file;

    source_file = fopen(assembly_path(path), "r");
    if (source_file == NULL) {
        printf("Failed to open file: %s. Skipping.\n", path);
        free(path);
        return false;
    }

    expanded_source_file = fopen(expanded_assembly_path(path), "w");
    success = pre_assembly(source_file, expanded_source_file);
    fclose(expanded_source_file);
    if (!success) {
        remove(expanded_assembly_path(path));
        return false;
    }
    
    expanded_source_file = fopen(expanded_assembly_path(path), "r");
    success = first_pass(expanded_source_file);
    fclose(expanded_source_file);
    if (!success) {
        printf("Uh oh\n");
        return false;
    }

    expanded_source_file = fopen(expanded_assembly_path(path), "r");
    success = second_pass(expanded_source_file);
    fclose(expanded_source_file);
    if (!success) {
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    int index;
    int file_count = argc - 1;
    char **file_names = argv + 1;

    for (index = 0; index < file_count; index++) {
        process_file(file_names[index]);
    }

    return SUCCESS;
}