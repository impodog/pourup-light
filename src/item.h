//
// Created by impodog on 8/5/2023.
//

#ifndef POURUP_LIGHT_ITEM_H
#define POURUP_LIGHT_ITEM_H

#include "const.h"
#include "syntax.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PUPL_Item_;
typedef struct PUPL_Item_ *PUPL_Item;
typedef PUPL_Item *PUPL_Array;
typedef PUPL_Array PUPL_ItemVec;

enum PUPL_ItemType {
    PUPL_INT_T,
    PUPL_PTR_T,
    PUPL_FLOAT_T,
    PUPL_BOOL_T,
    PUPL_NULL_T,
    PUPL_STRING_T,
    PUPL_ARRAY_T,
    PUPL_ENVIRON_PTR_T
};

struct PUPL_Item_ {
    enum PUPL_ItemType type;
    union {
        PUPL_Int int_v;
        PUPL_Ptr ptr_v;
        PUPL_Float float_v;
        PUPL_Bool bool_v;
        PUPL_String string_v;
        PUPL_Array array_v;
        PUPL_Ptr environ_ptr_v;
    } value;
};

PUPL_Item PUPL_Item_new_int(PUPL_Int value);

PUPL_Item PUPL_Item_new_ptr(PUPL_Ptr value);

PUPL_Item PUPL_Item_new_float(PUPL_Float value);

PUPL_Item PUPL_Item_new_bool(PUPL_Bool value);

PUPL_Item PUPL_Item_new_null();

PUPL_Item PUPL_Item_new_string(PUPL_ConstString value);

/* This function does NOT copy the array, it just copys the pointer to the array.
 * You should NOT free the array after calling this function. */
PUPL_Item PUPL_Item_new_array(PUPL_Array value);

PUPL_Item PUPL_Item_new_environ_ptr(PUPL_Ptr value);

PUPL_Ptr PUPL_Item_copy_environ_ptr(PUPL_Ptr ptr);

void PUPL_Item_free_environ_ptr(PUPL_Ptr ptr);

void PUPL_Item_show_environ_ptr(PUPL_Ptr ptr);

PUPL_Item PUPL_Item_copy(PUPL_Item item);

void PUPL_Item_free(PUPL_Item item);

void PUPL_Item_show(PUPL_Item item);

PUPL_Array PUPL_Array_new();

void PUPL_Array_push(PUPL_Array *array, PUPL_Item item);

/* NOTE: This function DOES free the items in the array.
 * Also, after calling PUPL_Item_new_array, you should NOT free the array using this function. */
void PUPL_Array_free(PUPL_Array array);

#ifdef __cplusplus
}
#endif
#endif //POURUP_LIGHT_ITEM_H
