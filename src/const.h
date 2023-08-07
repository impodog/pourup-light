//
// Created by impodog on 8/5/2023.
//

#ifndef POURUP_LIGHT_CONST_H
#define POURUP_LIGHT_CONST_H

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "ctype.h"
#include "limits.h"
#include "c-vector/vec.h"
#include "c-hashmap/map.h"

#ifndef strcpy_s
#define strcpy_s strncpy
#endif
#ifndef sprintf_s
#define sprintf_s snprintf
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PUPL_MAX_LINE_LENGTH 1024

#define PUPL_VERSION "0.3.1"


typedef char PUPL_Operator;
typedef char *PUPL_String;
typedef const char *PUPL_ConstString;
typedef PUPL_String *PUPL_StringVec;
typedef PUPL_ConstString *PUPL_ConstStringVec;
typedef hashmap *PUPL_HashMap;

typedef long long PUPL_Int;
typedef uintptr_t PUPL_Ptr;
typedef double PUPL_Float;
typedef unsigned char PUPL_Bool;


#ifdef __cplusplus
}
#endif

#endif //POURUP_LIGHT_CONST_H
