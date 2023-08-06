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
    env->stack = vector_create();
    env->result_v = env->left_bracket_v = NULL;
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
    if (hashmap_get(env->items, key, strlen(key), &prev)) {
        PUPL_Item item = (PUPL_Item) prev;
        if (item->type == PUPL_ENVIRON_PTR_T) {
            return (PUPL_Environ) item->value.environ_ptr_v;
        } else {
            return NULL;
        }
    } else {
        PUPL_Environ sub = PUPL_Environ_new();
        hashmap_set(env->items, key, strlen(key), (uintptr_t) PUPL_Item_new_environ_ptr((uintptr_t) sub));
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
    size_t length = strlen(key);
    switch (key[0]) {
        case '$':
            return PUPL_Item_new_string(key + 1);
        case '#':
            if (length < 2) {
                return NULL;
            }
            switch (key[1]) {
                case 'T':
                    return PUPL_Item_new_bool(1);
                case 'F':
                    return PUPL_Item_new_bool(0);
                case 'N':
                    return PUPL_Item_new_null();
                default:
                    return NULL;
            }
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

void PUPL_Environ_push_stack(PUPL_Environ env, PUPL_Item item) {
    vector_add(&env->stack, item);
}

PUPL_Item PUPL_Environ_pull_stack(PUPL_Environ env) {
    PUPL_Item result = env->stack[vector_size(env->stack) - 1];
    vector_pop(env->stack);
    return result;
}

void PUPL_Environ_set_result(PUPL_Environ env, PUPL_Item item) {
    PUPL_Item_free(env->result_v);
    env->result_v = item;
}

PUPL_Item PUPL_Environ_get_result(PUPL_Environ env) {
    PUPL_Item result = env->result_v;
    env->result_v = NULL;
    return result;
}

void PUPL_Environ_set_left_bracket(PUPL_Environ env, PUPL_Item item) {
    PUPL_Item_free(env->left_bracket_v);
    env->left_bracket_v = item;
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
}

void PUPL_Environ_copy_item(void *key, size_t ksize, uintptr_t value, void *usr) {
    PUPL_Environ_set((PUPL_Environ) usr, (PUPL_ConstString) key, PUPL_Item_copy((PUPL_Item) value));
}

void PUPL_Environ_copy_sub(void *key, size_t ksize, uintptr_t value, void *usr) {
    PUPL_Environ_set((PUPL_Environ) usr, (PUPL_ConstString) key,
                     PUPL_Item_new_environ_ptr(PUPL_Item_copy_environ_ptr((PUPL_Ptr) value)));
}

PUPL_Ptr PUPL_Item_copy_environ_ptr(PUPL_Ptr ptr) {
    PUPL_Environ env = PUPL_Environ_new();
    PUPL_Environ src = (PUPL_Environ) ptr;
    hashmap_iterate(src->items, PUPL_Environ_copy_item, env);
    return (PUPL_Ptr) env;
}

void PUPL_Item_free_environ_ptr(PUPL_Ptr ptr) {
    PUPL_Environ_free((PUPL_Environ) ptr);
}

void PUPL_Item_show_environ_ptr(PUPL_Ptr ptr) {
    PUPL_Environ_show((PUPL_Environ) ptr);
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
    PUPL_Environ prev = PUPL_CallStack_top(call_stack);
    vector_pop(call_stack->stack);
    if (prev->result_v) {
        PUPL_Environ top = PUPL_CallStack_top(call_stack);
        PUPL_Environ_push_stack(top, PUPL_Environ_get_result(prev));
    }
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