//
// Created by impodog on 8/5/2023.
//

#ifndef POURUP_LIGHT_ENVIRON_H
#define POURUP_LIGHT_ENVIRON_H

#include "item.h"

#ifdef __cplusplus
extern "C" {
#endif
struct PUPL_Environ_;
typedef struct PUPL_Environ_ *PUPL_Environ;
typedef PUPL_Environ *PUPL_EnvironVec;
struct PUPL_Environ_ {
    PUPL_HashMap items;

    PUPL_ItemVec stack;
    PUPL_Item result_v, left_bracket_v;
};

/* This function always returns a valid PUPL_Environ. */
PUPL_Environ PUPL_Environ_new();

void PUPL_Environ_free(PUPL_Environ env);

/* This does NOT copy the value. You may want to copy the value before calling this function.
 * This function will free the previous value if the key already exists. */
void PUPL_Environ_set(PUPL_Environ env, PUPL_ConstString key, PUPL_Item value);

/* This pops a key-value pair from the environment.
 * This function will does NOT free the value.
 * Returns the value if key exists, NULL otherwise. */
PUPL_Item PUPL_Environ_pop(PUPL_Environ env, PUPL_ConstString key);

/* Creates a sub-environment and returns it.
 * If the key already exists and a sub-environment is already created, then it returns the sub-environment.
 * If the key already exists and the value is not a sub-environment, then it returns NULL.
 * If the key does not exist, then it creates a sub-environment and returns it. */
PUPL_Environ PUPL_Environ_sub(PUPL_Environ env, PUPL_ConstString key);

/* Copies the environment recursively. */
PUPL_Environ PUPL_Environ_new_copy(PUPL_Environ src);

/* Copies the environment to a sub-environment recursively. */
PUPL_Environ PUPL_Environ_sub_copy(PUPL_Environ env, PUPL_Environ src, PUPL_ConstString key);

/*This function may return NULL when the key does not exist.
 * Remember that this does NOT return a copy.*/
PUPL_Item PUPL_Environ_find(PUPL_Environ env, PUPL_ConstString key);

/*This function may return NULL when the key does not exist.
 * Remember that this DOES return a copy of the value*/
PUPL_Item PUPL_Environ_find_value(PUPL_Environ env, PUPL_ConstString key);

/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_push_stack(PUPL_Environ env, PUPL_Item item);

/* This may return NULL when the stack is empty. */
PUPL_Item PUPL_Environ_pull_stack(PUPL_Environ env);

/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_set_result(PUPL_Environ env, PUPL_Item item);

PUPL_Item PUPL_Environ_get_result(PUPL_Environ env);

/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_set_left_bracket(PUPL_Environ env, PUPL_Item item);

void PUPL_Environ_show(PUPL_Environ env);


struct PUPL_CallStack_ {
    PUPL_EnvironVec stack;
};
typedef struct PUPL_CallStack_ *PUPL_CallStack;

PUPL_CallStack PUPL_CallStack_new();

void PUPL_CallStack_free(PUPL_CallStack call_stack);

/* This does NOT copy the env. You do NOT need to free the env again after calling this function. */
void PUPL_CallStack_push(PUPL_CallStack call_stack, PUPL_Environ env);

PUPL_Bool PUPL_CallStack_pop(PUPL_CallStack call_stack);

PUPL_Environ PUPL_CallStack_get(PUPL_CallStack call_stack, size_t index);

/* Returns the top of the stack, or the current environ. */
PUPL_Environ PUPL_CallStack_top(PUPL_CallStack call_stack);

/* Returns to the bottom of the stack, or the global environ. */
PUPL_Environ PUPL_CallStack_bottom(PUPL_CallStack call_stack);

size_t PUPL_CallStack_size(PUPL_CallStack call_stack);

#ifdef __cplusplus
};
#endif

#endif //POURUP_LIGHT_ENVIRON_H
