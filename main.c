#include "main.h"

/* using without free causes mem leak */
char *assembly_path(char *file_name) { 
    return concatenate(file_name, ".as");
}

/* using without free causes mem leak */
char *expanded_assembly_path(char *file_name) {
    return concatenate(file_name, ".am");
}

char *object_path(char *file_name) {
    return concatenate(file_name, ".ob");
}

char *entries_path(char *file_name) {
    return concatenate(file_name, ".ent");
}

char *externals_path(char *file_name) {
    return concatenate(file_name, ".ext");
}

bool process_file(char *path) {
    bool success;
    FILE *source_file, *expanded_source_file, *object_file;
    FILE *entries_file, *externals_file;

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
    printf("Time to write!\n");
    object_file = fopen(object_path(path), "w");
    write_image_output(object_file);
    fclose(object_file);

    if (entry_symbols_exist()) {
        entries_file = fopen(entries_path(path), "w");
        write_entry_symbols(entries_file);
        fclose(entries_file);
    }

    if (external_symbols_exist()) {
        externals_file = fopen(externals_path(path), "w");
        write_external_symbols(externals_file);
        fclose(externals_file);
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