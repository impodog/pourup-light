//
// Created by impodog on 8/5/2023.
//

#ifndef POURUP_LIGHT_SYNTAX_H
#define POURUP_LIGHT_SYNTAX_H

#include "const.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PUPL_Command_ {
    PUPL_Operator operator;
    PUPL_String operand;
};
typedef struct PUPL_Command_ *PUPL_Command;
typedef PUPL_Command *PUPL_CommandVec;

PUPL_Command PUPL_Command_new(PUPL_ConstString line, size_t length);

void PUPL_Command_free(PUPL_Command command);

void PUPL_Command_show(PUPL_Command command);


struct PUPL_Block_ {
    PUPL_CommandVec commands;
};
typedef struct PUPL_Block_ *PUPL_Block;

PUPL_Block PUPL_Block_new(PUPL_StringVec lines);

void PUPL_Block_free(PUPL_Block block);

void PUPL_Block_show(PUPL_Block block);


size_t PUPL_String_hash(PUPL_ConstString string);

PUPL_Bool PUPL_String_contains(PUPL_ConstString string, char ch);

void PUPL_String_format(PUPL_ConstString str, size_t str_size, PUPL_String buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif //POURUP_LIGHT_SYNTAX_H
