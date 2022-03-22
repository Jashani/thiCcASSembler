#include "machine_code.h"

binary_list code_image = NULL;
binary_list data_image = NULL;

int build_binary_instruction(encoding_type encoding, int functor,
                             int target_addressing, int target_register,
                             int source_addressing, int source_register) {
    int final_value = encoding;
    final_value |= (functor << FUNCTOR_LOCATION);
    final_value |= (addressing_to_value(target_addressing) << TARGET_ADDRESSING_LOCATION);
    final_value |= (target_register << TARGET_REGISTER_LOCATION);
    final_value |= (addressing_to_value(source_addressing) << SOURCE_ADDRESSING_LOCATION);
    final_value |= (source_register << SOURCE_REGISTER_LOCATION);
    printf("Built binary instruction for: %d, %d, %d, %d, %d - %05x\n", functor, source_register, addressing_to_value(source_addressing), target_register, addressing_to_value(target_addressing), final_value);
    return final_value;
}

/* Convert flag to real value */
int addressing_to_value(addressing to_convert) {
    switch (to_convert) {
        case ADDRESSING_IMMEDIATE:
            return 0;
        case ADDRESSING_DIRECT:
            return 1;
        case ADDRESSING_INDEX:
            return 2;
        case ADDRESSING_REGISTER:
            return 3;
    }
}

bool add_to_code_image(int data, char *future_label) {
    struct binary_node *new_node = (struct binary_node *)safe_malloc(sizeof(struct binary_node));
    struct binary_node *current_node;

    g_instruction_counter++;
    new_node->next = NULL;
    new_node->value = data;
    if (future_label != NULL && *future_label != '\0') {
        new_node->future_label = strdup(future_label);
    }

    printf("Adding to code image entry: %05x\n", data);
    if (code_image == NULL) {
        code_image = new_node;
        return true;
    }

    current_node = code_image;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
    return true;}

bool add_to_data_image(int data) {
    struct binary_node *new_node = (struct binary_node *)safe_malloc(sizeof(struct binary_node));
    struct binary_node *current_node;

    g_data_counter++;
    new_node->next = NULL;
    new_node->value = data;
    new_node->future_label = NULL;

    if (data_image == NULL) {
        data_image = new_node;
        return true;
    }

    current_node = data_image;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
    return true;
}

void print_data_image() {
    struct binary_node *current_node;
    current_node = data_image;
    while (current_node != NULL) {
        printf("Data image entry: %05x (%c)\n", current_node->value, current_node->value);
        current_node = current_node->next;
    }
}

void print_code_image() {
    struct binary_node *current_node;
    current_node = code_image;
    while (current_node != NULL) {
        printf("Code image entry: %05x - %s\n", current_node->value, current_node->future_label);
        current_node = current_node->next;
    }
}

bool populate_symbols() {
    int current_base, current_offset;
    struct binary_node *current_node;
    current_node = code_image;
    while (current_node != NULL) {
        if (current_node->future_label != NULL) {
            if (!symbol_address(current_node->future_label, &current_base, &current_offset)) {
                return false;
            }
            current_node->value = current_base;
            current_node = current_node->next;
            current_node->value = current_offset;
        }

        current_node = current_node->next;
    }
    return true;
}
