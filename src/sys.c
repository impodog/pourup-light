//
// Created by impodog on 8/6/2023.
//

#include "sys.h"

void PUPL_init() {
    PUPL_Operation_init();
    PUPL_FunctionMap_init();
}

void PUPL_quit() {
    PUPL_FunctionMap_free();
    PUPL_Operation_free();
}

PUPL_Block PUPL_Block_parse(const char *filename) {
    PUPL_StringVec lines = vector_create();
    PUPL_Block block;

    if (!PUPL_read_lines(filename, &lines)) {
        block = PUPL_Block_new(lines);
        PUPL_free_lines(lines);

        return block;
    }
    return NULL;
}

PUPL_Bool PUPL_run(PUPL_CallStack call_stack, PUPL_Block block) {
    size_t i, length = vector_size(block->commands);
    for (i = 0; i < length; ++i) {
        if (PUPL_Operation_call(call_stack, block->commands[i])) {
            return 1;
        }
    }
    return 0;
}

PUPL_CallStack PUPL_parse(const char *filename) {
    PUPL_CallStack call_stack = PUPL_CallStack_new();
    PUPL_Block block = PUPL_Block_parse(filename);
    if (!block) {
        char buf[PUPL_MAX_LINE_LENGTH];
        sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "Cannot parse file: %s", filename);
        PUPL_set_error(buf);
        return NULL;
    }
    if (PUPL_run(call_stack, block)) {
        PUPL_Block_free(block);
        return NULL;
    }
    PUPL_Block_free(block);
    return call_stack;
}