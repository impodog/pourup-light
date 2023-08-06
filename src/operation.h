//
// Created by impodog on 8/6/2023.
//

#ifndef POURUP_LIGHT_OPERATION_H
#define POURUP_LIGHT_OPERATION_H

#include "environ.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PUPL_Bool (*PUPL_Operation)(PUPL_CallStack call_stack, PUPL_String arg);

typedef PUPL_Bool (*PUPL_Function)(PUPL_CallStack call_stack);

typedef PUPL_Operation *PUPL_OperationTable;
typedef hashmap *PUPL_FunctionMap;

void PUPL_set_error(const char *msg);

const char *PUPL_get_error();

void PUPL_Operation_init();

void PUPL_Operation_free();

void PUPL_Operation_add(char op, PUPL_Operation operation);

/* Returns 1 on failure(operation not found), or 0 on success */
PUPL_Bool PUPL_Operation_call(PUPL_CallStack call_stack, PUPL_Command command);

void PUPL_FunctionMap_init();

void PUPL_FunctionMap_free();

/* Returns some pointer to the original function if the function exists, or NULL if not */
PUPL_Function PUPL_FunctionMap_add(PUPL_ConstString name, PUPL_Function function);

/* Returns NULL on failure(function not found) */
PUPL_Function PUPL_FunctionMap_find(PUPL_ConstString name);

#ifdef __cplusplus
};
#endif

#endif //POURUP_LIGHT_OPERATION_H
