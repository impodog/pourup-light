//
// Created by impodog on 8/6/2023.
//

#include "operation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PUPL_OPERATION_TABLE_SIZE (CHAR_MAX + 3)

PUPL_Operation PUPL_operations[PUPL_OPERATION_TABLE_SIZE];
PUPL_FunctionMap PUPL_functions = NULL;
char *PUPL_error = NULL;

PUPL_Bool PUPL_op_stack_push(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    PUPL_Item item = PUPL_Environ_find_value(env, arg);
    if (item) {
        PUPL_Environ_push_stack(env, item);
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When pushing \"%s\" into stack, item not found", arg);
    PUPL_set_error(buf);
    return 1;
}

PUPL_Bool PUPL_op_stack_pull(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    size_t length = vector_size(env->stack);
    if (length) {
        PUPL_Environ_set(env, arg, PUPL_Environ_pull_stack(env));
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When pulling into \"%s\", stack is empty", arg);
    PUPL_set_error(buf);
    return 1;
}

PUPL_Bool PUPL_op_show(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    PUPL_Item item = PUPL_Environ_find(env, arg);
    if (item) {
        PUPL_Item_show(item);
        putchar('\n');
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When using Show operation, item \"%s\" not found", arg);
    PUPL_set_error(buf);
    return 1;
}

PUPL_Bool PUPL_op_stack_enter(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    PUPL_Environ sub = PUPL_Environ_sub(env, arg);
    PUPL_CallStack_push(call_stack, sub);
    return 0;
}

PUPL_Bool PUPL_op_stack_leave(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    if (PUPL_CallStack_pop(call_stack)) {
        PUPL_set_error("When leaving sub stack, sub stack is empty");
        return 1;
    }
    return 0;
}

PUPL_Bool PUPL_op_run_function(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Function function = PUPL_FunctionMap_find(arg);
    if (function) {
        return function(call_stack);
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When running function \"%s\", function not found", arg);
    PUPL_set_error(buf);
    return 1;
}

PUPL_Bool PUPL_op_left_bracket(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    PUPL_Item item = PUPL_Environ_find_value(env, arg);
    if (item) {
        PUPL_Environ_set_left_bracket(env, item);
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When using left bracket, item \"%s\" not found", arg);
    PUPL_set_error(buf);
    return 1;
}

PUPL_Bool PUPL_op_right_bracket(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    size_t length = vector_size(env->stack);
    if (env->left_bracket_v->type == PUPL_INTEGER_T) {
        size_t size = env->left_bracket_v->value.integer_v;
        if (length >= size) {
            PUPL_Array arr = PUPL_Array_new();
            size_t i;
            for (i = 0; i < size; ++i) {
                PUPL_Array_push(&arr, PUPL_Environ_pull_stack(env));
            }
            PUPL_Environ_set(env, arg, PUPL_Item_new_array(arr));
            return 0;
        } else {
            char buf[PUPL_MAX_LINE_LENGTH];
            sprintf_s(buf, PUPL_MAX_LINE_LENGTH,
                      "When using right bracket, stack size is %d, but left bracket(or desired) size is %d",
                      length, size);
            PUPL_set_error(buf);
            return 1;
        }
    } else {
        char buf[PUPL_MAX_LINE_LENGTH];
        sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When using right bracket, left bracket is not integer");
        PUPL_set_error(buf);
        return 1;
    }
}

PUPL_Bool PUPL_op_return(PUPL_CallStack call_stack, PUPL_ConstString arg) {
    PUPL_Environ env = PUPL_CallStack_top(call_stack);
    PUPL_Item result = PUPL_Environ_find_value(env, arg);
    if (result) {
        PUPL_Environ_set_result(env, result);
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "When using return, item \"%s\" not found", arg);
    PUPL_set_error(buf);
    return 1;
}

void PUPL_Operation_init() {
    size_t i;
    memset(PUPL_operations, 0, PUPL_OPERATION_TABLE_SIZE * sizeof(PUPL_Operation));
    PUPL_operations['<'] = PUPL_op_stack_push;
    PUPL_operations['>'] = PUPL_op_stack_pull;
    PUPL_operations['S'] = PUPL_op_show;
    PUPL_operations['{'] = PUPL_op_stack_enter;
    PUPL_operations['}'] = PUPL_op_stack_leave;
    PUPL_operations['.'] = PUPL_op_run_function;
    PUPL_operations['['] = PUPL_op_left_bracket;
    PUPL_operations[']'] = PUPL_op_right_bracket;
    PUPL_operations['='] = PUPL_op_return;
}

void PUPL_Operation_free() {
    size_t i;
    for (i = 0; i < PUPL_OPERATION_TABLE_SIZE; ++i) {

    }
}

void PUPL_Operation_add(char op, PUPL_Operation operation) {
    PUPL_operations[op] = operation;
}

PUPL_Bool PUPL_Operation_call(PUPL_CallStack call_stack, PUPL_Command command) {
    PUPL_Operation operation = PUPL_operations[command->operator];
    if (operation) {
        if (operation(call_stack, command->operand)) {
            return 1;
        }
        return 0;
    }
    char buf[PUPL_MAX_LINE_LENGTH];
    sprintf_s(buf, PUPL_MAX_LINE_LENGTH, "Operation '%c' not found", command->operator);
    PUPL_set_error(buf);
    return 1;
}

void PUPL_FunctionMap_init() {
    PUPL_functions = hashmap_create();
}

void PUPL_FunctionMap_free() {
    hashmap_free(PUPL_functions);
}

PUPL_Function PUPL_FunctionMap_add(PUPL_ConstString name, PUPL_Function function) {
    uintptr_t prev = 0;
    hashmap_get(PUPL_functions, name, strlen(name), &prev);
    hashmap_set(PUPL_functions, name, strlen(name), (uintptr_t) function);
    return (PUPL_Function) prev;
}

PUPL_Function PUPL_FunctionMap_find(PUPL_ConstString name) {
    uintptr_t result;
    if (hashmap_get(PUPL_functions, name, strlen(name), &result)) {
        return (PUPL_Function) result;
    }
    return NULL;
}

void PUPL_set_error(const char *msg) {
    if (PUPL_error)
        free(PUPL_error);
    PUPL_error = strdup(msg);
}

const char *PUPL_get_error() {
    return PUPL_error;
}

#ifdef __cplusplus
};
#endif
