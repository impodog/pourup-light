//
// Created by impodog on 8/6/2023.
//

#ifndef POURUP_LIGHT_SYS_H
#define POURUP_LIGHT_SYS_H

#include "operation.h"
#include "reader.h"

void PUPL_init();

void PUPL_quit();

/*This returns NULL when the file does not exist, or pupl cannot parse the file.*/
PUPL_Block PUPL_Block_parse(const char *filename);

PUPL_Bool PUPL_run(PUPL_CallStack call_stack, PUPL_Block block);

/*This returns NULL when the file does not exist, pupl cannot parse the file, or the file causes an error.\
 * It is recommended to use this to parse a .pupl file*/
PUPL_CallStack PUPL_parse(const char *filename);

#endif //POURUP_LIGHT_SYS_H
