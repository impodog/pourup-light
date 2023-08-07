//
// Created by impodog on 8/5/2023.
//

#include "item.h"

#ifdef __cplusplus
extern "C" {
#endif

PUPL_Item PUPL_Item_new_int(PUPL_Int value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_INT_T;
    item->value.int_v = value;
    return item;
}

PUPL_Item PUPL_Item_new_ptr(PUPL_Ptr value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_PTR_T;
    item->value.ptr_v = value;
    return item;
}

PUPL_Item PUPL_Item_new_float(PUPL_Float value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_FLOAT_T;
    item->value.float_v = value;
    return item;
}

PUPL_Item PUPL_Item_new_bool(PUPL_Bool value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_BOOL_T;
    item->value.bool_v = value;
    return item;
}

PUPL_Item PUPL_Item_new_null() {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_NULL_T;
    return item;
}

PUPL_Item PUPL_Item_new_string(PUPL_ConstString value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    size_t length = strlen(value);
    item->type = PUPL_STRING_T;
    item->value.string_v = (char *) malloc(length);
    PUPL_String_format(value, strlen(value), item->value.string_v, length);
    return item;
}

PUPL_Item PUPL_Item_new_array(PUPL_Array value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_ARRAY_T;
    item->value.array_v = value;
    return item;
}

PUPL_Item PUPL_Item_new_environ_ptr(PUPL_Ptr value) {
    PUPL_Item item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
    item->type = PUPL_ENVIRON_PTR_T;
    item->value.environ_ptr_v = value;
    return item;
}

PUPL_Item PUPL_Item_copy(PUPL_Item item) {
    if (item) {
        PUPL_Item new_item = (PUPL_Item) malloc(sizeof(struct PUPL_Item_));
        new_item->type = item->type;
        switch (item->type) {
            case PUPL_INT_T:
                new_item->value.int_v = item->value.int_v;
                break;
            case PUPL_PTR_T:
                new_item->value.ptr_v = item->value.ptr_v;
                break;
            case PUPL_FLOAT_T:
                new_item->value.float_v = item->value.float_v;
                break;
            case PUPL_BOOL_T:
                new_item->value.bool_v = item->value.bool_v;
                break;
            case PUPL_NULL_T:
                break;
            case PUPL_STRING_T:
                new_item->value.string_v = strdup(item->value.string_v);
                break;
            case PUPL_ARRAY_T:
                new_item->value.array_v = vector_create();
                {
                    size_t i, length;
                    length = vector_size(item->value.array_v);
                    for (i = 0; i < length; ++i) {
                        vector_add(&new_item->value.array_v, PUPL_Item_copy(item->value.array_v[i]));
                    }
                }
                break;
            case PUPL_ENVIRON_PTR_T:
                new_item->value.environ_ptr_v = PUPL_Item_copy_environ_ptr(item->value.environ_ptr_v);
                break;
        }
        return new_item;
    }
    return NULL;
}

void PUPL_Item_free(PUPL_Item item) {
    if (item) {
        switch (item->type) {
            case PUPL_INT_T:
            case PUPL_PTR_T:
            case PUPL_FLOAT_T:
            case PUPL_BOOL_T:
            case PUPL_NULL_T:
                break;
            case PUPL_STRING_T:
                free(item->value.string_v);
                break;
            case PUPL_ARRAY_T:
                PUPL_Array_free(item->value.array_v);
                break;
            case PUPL_ENVIRON_PTR_T:
                PUPL_Item_free_environ_ptr(item->value.environ_ptr_v);
                break;
        }
        free(item);
    }
}

void PUPL_Item_show(PUPL_Item item) {
    switch (item->type) {
        case PUPL_INT_T:
            printf("%lld", item->value.int_v);
            break;
        case PUPL_PTR_T:
            printf("0x%llx", item->value.ptr_v);
            break;
        case PUPL_FLOAT_T:
            printf("%f", item->value.float_v);
            break;
        case PUPL_BOOL_T:
            printf("%s", item->value.bool_v ? "true" : "false");
            break;
        case PUPL_NULL_T:
            printf("null");
            break;
        case PUPL_STRING_T:
            printf("\"%s\"", item->value.string_v);
            break;
        case PUPL_ARRAY_T: {
            size_t i, length;
            length = vector_size(item->value.array_v);
            printf("[");
            for (i = 0; i < length; ++i) {
                PUPL_Item_show((PUPL_Item) item->value.array_v[i]);
                if (i != length - 1) {
                    printf(", ");
                }
            }
            printf("]");
        }
            break;
        case PUPL_ENVIRON_PTR_T:
            PUPL_Item_show_environ_ptr(item->value.environ_ptr_v);
            break;
    }
}

PUPL_Array PUPL_Array_new() {
    return vector_create();
}

void PUPL_Array_push(PUPL_Array *array, PUPL_Item item) {
    vector_add(array, item);
}

void PUPL_Array_free(PUPL_Array array) {
    size_t i, length;
    length = vector_size(array);
    for (i = 0; i < length; ++i) {
        PUPL_Item_free((PUPL_Item) array[i]);
    }
    vector_free(array);
}

#ifdef __cplusplus
}
#endif