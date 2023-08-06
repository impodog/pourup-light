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
    PUPL_INTEGER_T,
    PUPL_PTR_T,
    PUPL_FLOAT_T,
    PUPL_BOOL_T,
    PUPL_NULL_T,
    PUPL_STRING_T,
    PUPL_ARRAY_T
};

struct PUPL_Item_ {
    enum PUPL_ItemType type;
    union {
        PUPL_Integer integer_v;
        PUPL_Ptr ptr_v;
        PUPL_Float float_v;
        PUPL_Bool bool_v;
        PUPL_String string_v;
        PUPL_Array array_v;
    } value;
};

PUPL_Item PUPL_Item_new_integer(PUPL_Integer value);

PUPL_Item PUPL_Item_new_ptr(PUPL_Ptr value);

PUPL_Item PUPL_Item_new_float(PUPL_Float value);

PUPL_Item PUPL_Item_new_bool(PUPL_Bool value);

PUPL_Item PUPL_Item_new_null();

PUPL_Item PUPL_Item_new_string(PUPL_ConstString value);

PUPL_Item PUPL_Item_new_array(PUPL_Array value);

PUPL_Item PUPL_Item_copy(PUPL_Item item);

void PUPL_Item_free(PUPL_Item item);

void PUPL_Item_show(PUPL_Item item);

PUPL_Array PUPL_Array_new();

void PUPL_Array_push(PUPL_Array *array, PUPL_Item item);

void PUPL_Array_free(PUPL_Array array);

#ifdef __cplusplus
}
#endif
#endif //POURUP_LIGHT_ITEM_H