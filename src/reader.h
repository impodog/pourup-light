//
// Created by impodog on 8/6/2023.
//

#ifndef POURUP_LIGHT_READER_H
#define POURUP_LIGHT_READER_H

#include "environ.h"

#ifdef __cplusplus
extern "C" {
#endif

void PUPL_readline(FILE *file, char *buf, size_t size);

/*Returns 1 on failure(file not found), or 0 on success*/
PUPL_Bool PUPL_read_lines(const char *filename, PUPL_StringVec *lines);

void PUPL_free_lines(PUPL_StringVec lines);


#ifdef __cplusplus
};
#endif
#endif //POURUP_LIGHT_READER_H
