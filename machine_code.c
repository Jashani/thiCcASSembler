#include "common.h"
#include "machine_code.h"

binary_list code_image = NULL;
binary_list data_image = NULL;



bool add_to_data_image(int data) {
    struct binary_node *new_node = (struct binary_node *)safe_malloc(sizeof(struct binary_node));
    struct binary_node *current_node;

    new_node->next = NULL;
    new_node->value = data;

    if (data_image == NULL) {
        data_image = new_node;
        return true;
    }

    current_node = data_image;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
}

void print_data_image() {
    struct binary_node *current_node;
    current_node = data_image;
    while (current_node != NULL) {
        printf("Data image entry: %d (%c)\n", current_node->value, current_node->value);
        current_node = current_node->next;
    }
}