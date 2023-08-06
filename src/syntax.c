//
// Created by impodog on 8/5/2023.
//

#include "syntax.h"

#ifdef __cplusplus
extern "C" {
#endif

PUPL_Command PUPL_Command_new(PUPL_ConstString line, size_t length) {
    PUPL_Command result = malloc(sizeof(struct PUPL_Command_));
    size_t pos = 0;
    while (isspace(line[pos])) {
        if (++pos == length) {
            free(result);
            return NULL;
        }
    }
    result->operator = line[pos];
    // The length is the length of the string, not the length of the memory.
    // To copy the NUL character, length is not subtracted by 1.
    result->operand = (PUPL_String) malloc(length);
    strcpy_s(result->operand, length, line + pos + 1);
    return result;
}

void PUPL_Command_free(PUPL_Command command) {
    free(command->operand);
    free(command);
}

void PUPL_Command_show(PUPL_Command command) {
    printf("Command{\'%c\', \"%s\"}\n", command->operator, command->operand);
}


PUPL_Block PUPL_Block_new(PUPL_StringVec lines) {
    PUPL_Block result = malloc(sizeof(struct PUPL_Block_));
    size_t i, length = vector_size(lines);

    result->commands = vector_create();
    for (i = 0; i < length; ++i) {
        size_t len = strlen(lines[i]);
        if (len == 0) continue;
        PUPL_Command cmd = PUPL_Command_new(lines[i], len);
        if (cmd) {
            vector_add(&result->commands, cmd);
        }
    }
    return result;
}

void PUPL_Block_free(PUPL_Block block) {
    size_t i, length = vector_size(block->commands);
    for (i = 0; i < length; ++i) {
        PUPL_Command_free(block->commands[i]);
    }
    vector_free(block->commands);
    free(block);
}

void PUPL_Block_show(PUPL_Block block) {
    size_t i, length = vector_size(block->commands);
    printf("Block 0x%llx:\n", (unsigned long long) block);
    for (i = 0; i < length; ++i) {
        PUPL_Command_show(block->commands[i]);
    }
}

size_t PUPL_String_hash(PUPL_ConstString string) {
    size_t hash = 0;
    for (size_t i = 0; i < strlen(string); ++i) {
        hash = hash * 31 + string[i];
    }
    return hash;
}

PUPL_Bool PUPL_String_contains(PUPL_ConstString string, char ch) {
    for (size_t i = 0; i < strlen(string); ++i) {
        if (string[i] == ch) {
            return 1;
        }
    }
    return 0;
}

void PUPL_String_format(PUPL_ConstString str, size_t str_size, PUPL_String buf, size_t buf_size) {
    PUPL_Bool backslash = 0;
    size_t i, j;
    for (i = 0, j = 0; i < str_size && j < buf_size; ++i) {
        if (backslash) {
            switch (str[i]) {
                case 'n':
                    buf[j++] = '\n';
                    break;
                case 't':
                    buf[j++] = '\t';
                    break;
                case 'r':
                    buf[j++] = '\r';
                    break;
                case '\\':
                    buf[j++] = '\\';
                    break;
                default:
                    buf[j++] = str[i];
                    break;
            }
            backslash = 0;
        } else {
            if (str[i] == '\\') {
                backslash = 1;
            } else {
                buf[j++] = str[i];
            }
        }
    }
    buf[j] = '\0';
}

#ifdef __cplusplus
}
#endif



