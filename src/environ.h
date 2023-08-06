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

PUPL_Environ PUPL_Environ_new();

void PUPL_Environ_free(PUPL_Environ env);

void PUPL_Environ_set(PUPL_Environ env, PUPL_ConstString key, PUPL_Item value);

PUPL_Environ PUPL_Environ_sub(PUPL_Environ env, PUPL_ConstString key);

/*This function may return NULL when the key does not exist.
 * Remember that this does NOT return a copy.*/
PUPL_Item PUPL_Environ_find(PUPL_Environ env, PUPL_ConstString key);

/*This function may return NULL when the key does not exist.
 * Remember that this DOES return a copy of the value*/
PUPL_Item PUPL_Environ_find_value(PUPL_Environ env, PUPL_ConstString key);

void PUPL_Environ_push_stack(PUPL_Environ env, PUPL_Item item);

PUPL_Item PUPL_Environ_pull_stack(PUPL_Environ env);

void PUPL_Environ_set_result(PUPL_Environ env, PUPL_Item item);

PUPL_Item PUPL_Environ_get_result(PUPL_Environ env);

void PUPL_Environ_set_left_bracket(PUPL_Environ env, PUPL_Item item);

void PUPL_Environ_show(PUPL_Environ env);


struct PUPL_CallStack_ {
    PUPL_EnvironVec stack;
};
typedef struct PUPL_CallStack_ *PUPL_CallStack;

PUPL_CallStack PUPL_CallStack_new();

void PUPL_CallStack_free(PUPL_CallStack call_stack);

PUPL_Environ PUPL_CallStack_top(PUPL_CallStack call_stack);

void PUPL_CallStack_push(PUPL_CallStack call_stack, PUPL_Environ env);

PUPL_Bool PUPL_CallStack_pop(PUPL_CallStack call_stack);

PUPL_Environ PUPL_CallStack_get(PUPL_CallStack call_stack, size_t index);

PUPL_Environ PUPL_CallStack_top(PUPL_CallStack call_stack);

PUPL_Environ PUPL_CallStack_bottom(PUPL_CallStack call_stack);

#ifdef __cplusplus
};
#endif

#endif //POURUP_LIGHT_ENVIRON_H
