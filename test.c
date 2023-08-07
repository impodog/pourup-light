//
// Created by impodog on 8/5/2023.
//
#include "pupl.h"

#ifdef _WIN32
#define PATH_SEP "\\"
#define EXE_SUFFIX ".exe"
#else
#define PATH_SEP "/"
#define EXE_SUFFIX ""
#endif
#define PPYL "." PATH_SEP "ppyl" PATH_SEP "cmake-build-debug" PATH_SEP "ppyl" EXE_SUFFIX

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    system(PPYL " test/test.json");
    PUPL_init();
    PUPL_CallStack call_stack = PUPL_parse(argv[1]);
    if (call_stack == NULL) {
        printf("Error: %s\n", PUPL_get_error());
        return 1;
    }
    PUPL_Environ_show(PUPL_CallStack_bottom(call_stack));
    PUPL_CallStack_free(call_stack);
    PUPL_quit();
    return 0;
}