#include "assembler.h"

char *assembly_path(char *file_name) { 
    return concatenate(file_name, ".as");
}

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

/* On given path, run pre assembly, first pass, and second pass.
 * After processing, export derived data to files.
 */
bool process_file(char *path) {
    bool success;
    char *path_container;
    FILE *source_file, *expanded_source_file, *object_file;
    FILE *entries_file, *externals_file;

    path_container = assembly_path(path);
    source_file = fopen(path_container, "r");
    free(path_container);
    if (source_file == NULL) {
        printf("Failed to open file: %s. Skipping.\n", path);
        return false;
    }

    path_container = expanded_assembly_path(path);
    expanded_source_file = fopen(path_container, "w");
    success = pre_assembly(source_file, expanded_source_file);
    fclose(source_file);
    fclose(expanded_source_file);
    if (!success) {
        remove(path_container);
        free(path_container);
        return false;
    }
    free(path_container);

    path_container = expanded_assembly_path(path);
    expanded_source_file = fopen(path_container, "r");
    success = first_pass(expanded_source_file);
    fclose(expanded_source_file);
    free(path_container);
    if (!success) {
        return false;
    }

    path_container = expanded_assembly_path(path);
    expanded_source_file = fopen(path_container, "r");
    success = second_pass(expanded_source_file);
    fclose(expanded_source_file);
    free(path_container); 
    if (!success) {
        return false;
    }

    path_container = object_path(path);
    object_file = fopen(path_container, "w");
    write_image_output(object_file);
    fclose(object_file);
    free(path_container);

    if (entry_symbols_exist()) {
        path_container = entries_path(path);
        entries_file = fopen(path_container, "w");
        write_entry_symbols(entries_file);
        fclose(entries_file);
        free(path_container);
    }

    if (external_symbols_exist()) {
        path_container = externals_path(path);
        externals_file = fopen(path_container, "w");
        write_external_symbols(externals_file);
        fclose(externals_file);
        free(path_container);
    }

    return true;
}

/* Process all files provided in arguments. Clean up after each file. */
int main(int argc, char *argv[]) {
    int index;
    int success = SUCCESS;
    int file_count = argc - 1;
    char **file_names = argv + 1;

    for (index = 0; index < file_count; index++) {
        if (!process_file(file_names[index])) {
            success = FAILURE;
        }
        clear_symbols();
        clear_images();
    }

    return success;
}