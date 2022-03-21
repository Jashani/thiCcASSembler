
#define FUNCTOR_LOCATION 12

typedef enum encoding_type {
    ABSOLUTE = 1 << 18,
    RELOCATABLE = 1 << 17,
    EXTERNAL = 1 << 16
} encoding_type;

typedef enum functor_value {
    FUNCTOR_VALUE_ADD = 10,
    FUNCTOR_VALUE_SUB = 11,
    FUNCTOR_VALUE_CLR = 10,
    FUNCTOR_VALUE_NOT = 11,
    FUNCTOR_VALUE_INC = 12,
    FUNCTOR_VALUE_DEC = 13,
    FUNCTOR_VALUE_JMP = 10,
    FUNCTOR_VALUE_BNE = 11,
    FUNCTOR_VALUE_JNR = 12
} functor_value;

typedef enum opcode_value {
    OPCODE_VALUE_MOV = 0,
    OPCODE_VALUE_CMP = 1,
    OPCODE_VALUE_ADD = 2,
    OPCODE_VALUE_SUB = 2,
    OPCODE_VALUE_LEA = 4,
    OPCODE_VALUE_CLR = 5,
    OPCODE_VALUE_NOT = 5,
    OPCODE_VALUE_INC = 5,
    OPCODE_VALUE_DEC = 5,
    OPCODE_VALUE_JMP = 9,
    OPCODE_VALUE_BNE = 9,
    OPCODE_VALUE_JSR = 9,
    OPCODE_VALUE_RED = 12,
    OPCODE_VALUE_PRN = 13,
    OPCODE_VALUE_RTS = 14,
    OPCODE_VALUE_STOP = 15
} opcode_value;

typedef struct binary_node {
    struct binary_node *next;
    int value;
} binary_node;

typedef binary_node* binary_list;

bool add_to_code_image(char *instruction, int source_registry, int source_addressing,
                       int target_registry, int target_addressing);
bool add_to_data_image(int data);
void print_data_image();