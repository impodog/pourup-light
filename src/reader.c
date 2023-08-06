//
// Created by impodog on 8/6/2023.
//

#include "reader.h"

#ifdef __cplusplus
extern "C" {
#endif

void PUPL_readline(FILE *file, char *buf, size_t size) {
    char ch;
    size_t i = 0;
    while ((ch = (char) fgetc(file)) != EOF && ch != '\n' && i < size) {
        buf[i++] = ch;
    }
    buf[i] = '\0';
}

PUPL_Bool PUPL_read_lines(const char *filename, PUPL_StringVec *lines) {
    char buf[PUPL_MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }
    while (!feof(file)) {
        PUPL_readline(file, buf, PUPL_MAX_LINE_LENGTH);
        vector_add(lines, strdup(buf));
    }
    fclose(file);
    return 0;
}

void PUPL_free_lines(PUPL_StringVec lines) {
    size_t i, length = vector_size(lines);
    for (i = 0; i < length; ++i) {
        free(lines[i]);
    }
    vector_free(lines);
}

#ifdef __cplusplus
};
#endif