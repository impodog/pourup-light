//
// Created by impodog on 8/5/2023.
//

#include "environ.h"

#ifdef __cplusplus
extern "C" {
#endif


PUPL_Environ PUPL_Environ_new() {
    PUPL_Environ env = (PUPL_Environ) malloc(sizeof(struct PUPL_Environ_));
    env->items = hashmap_create();
    env->sub = hashmap_create();
    env->stack = vector_create();
    return env;
}

void PUPL_Environ_item_free(void *key, size_t ksize, uintptr_t value, void *usr) {
    PUPL_Item_free((PUPL_Item) value);
}

void PUPL_Environ_sub_free(void *key, size_t ksize, uintptr_t value, void *usr) {
    PUPL_Environ_free((PUPL_Environ) value);
}

void PUPL_Environ_free(PUPL_Environ env) {
    hashmap_iterate(env->items, PUPL_Environ_item_free, NULL);
    hashmap_free(env->items);

    hashmap_iterate(env->sub, PUPL_Environ_sub_free, NULL);
    hashmap_free(env->sub);
    {
        size_t i, length = vector_size(env->stack);
        for (i = 0; i < length; ++i) {
            PUPL_Item_free(env->stack[i]);
        }
        vector_free(env->stack);
    }
    free(env);
}

void PUPL_Environ_set(PUPL_Environ env, PUPL_ConstString key, PUPL_Item value) {
    uintptr_t prev;
    size_t key_len = strlen(key);
    if (hashmap_get(env->items, key, key_len, &prev)) {
        PUPL_Item_free((PUPL_Item) prev);
    }
    hashmap_set(env->items, key, key_len, (uintptr_t) value);
}

PUPL_Environ PUPL_Environ_sub(PUPL_Environ env, PUPL_ConstString key) {
    uintptr_t prev;
    if (hashmap_get(env->sub, key, strlen(key), &prev)) {
        return (PUPL_Environ) prev;
    } else {
        PUPL_Environ sub = PUPL_Environ_new();
        hashmap_set(env->sub, key, strlen(key), (uintptr_t) sub);
        return sub;
    }
}

PUPL_Item PUPL_Environ_find(PUPL_Environ env, PUPL_ConstString key) {
    uintptr_t index;
    if (hashmap_get(env->items, key, strlen(key), &index)) {
        return (PUPL_Item) index;
    } else {
        return NULL;
    }
}

PUPL_Item PUPL_Environ_find_value(PUPL_Environ env, PUPL_ConstString key) {
    switch (key[0]) {
        case '$':
            return PUPL_Item_new_string(key + 1);
        case '0':
            return PUPL_Item_new_ptr(strtoll(key + 1, NULL, 16));
        default:
            if (isdigit(key[0])) {
                if (PUPL_String_contains(key, '.')) {
                    return PUPL_Item_new_float(strtod(key, NULL));
                } else {
                    return PUPL_Item_new_integer(strtoll(key, NULL, 10));
                }
            } else {
                return PUPL_Item_copy(PUPL_Environ_find(env, key));
            }
    }
}

void PUPL_Environ_show_item(void *key, size_t ksize, uintptr_t value, void *usr) {
    printf("%s: ", (PUPL_ConstString) key);
    PUPL_Item_show((PUPL_Item) value);
    putchar('\n');
}

void PUPL_Environ_show_sub(void *key, size_t ksize, uintptr_t value, void *usr) {
    printf("%s: {\n", (PUPL_ConstString) key);
    PUPL_Environ_show((PUPL_Environ) value);
    puts("}");
}

void PUPL_Environ_show(PUPL_Environ env) {
    printf("PUPL_Environ 0x%llx:\n", (uintptr_t) env);
    hashmap_iterate(env->items, PUPL_Environ_show_item, NULL);
    hashmap_iterate(env->sub, PUPL_Environ_show_sub, NULL);
}


PUPL_CallStack PUPL_CallStack_new() {
    PUPL_CallStack call_stack = (PUPL_CallStack) malloc(sizeof(struct PUPL_CallStack_));
    call_stack->stack = vector_create();
    vector_add(&call_stack->stack, PUPL_Environ_new());
    return call_stack;
}

void PUPL_CallStack_free(PUPL_CallStack call_stack) {
    PUPL_Environ_free(PUPL_CallStack_bottom(call_stack));
    vector_free(call_stack->stack);
    free(call_stack);
}

void PUPL_CallStack_push(PUPL_CallStack call_stack, PUPL_Environ env) {
    vector_add(&call_stack->stack, env);
}

PUPL_Bool PUPL_CallStack_pop(PUPL_CallStack call_stack) {
    if (vector_size(call_stack->stack) == 1) {
        return 1;
    }
    vector_pop(call_stack->stack);
    return 0;
}

PUPL_Environ PUPL_CallStack_get(PUPL_CallStack call_stack, size_t index) {
    return call_stack->stack[index];
}

PUPL_Environ PUPL_CallStack_top(PUPL_CallStack call_stack) {
    return call_stack->stack[vector_size(call_stack->stack) - 1];
}

PUPL_Environ PUPL_CallStack_bottom(PUPL_CallStack call_stack) {
    return call_stack->stack[0];
}

#ifdef __cplusplus
}
#endif