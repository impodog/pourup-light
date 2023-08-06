//
// Created by impodog on 8/5/2023.
//
#include "pupl.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    PUPL_init();
    PUPL_CallStack call_stack = PUPL_parse(argv[1]);
    if (call_stack == NULL) {
        printf("Error: %s\n", PUPL_get_error());
        return 1;
    }
    PUPL_quit();
    return 0;
}