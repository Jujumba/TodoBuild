#include "detector.h"

char *comp = NULL;

bool detect() {
    printf("Detecting C compiler...\n");
    if (system("clang --version") == 0) {
        comp = "clang";
    } else if (system("gcc --version") == 0) {
        comp = "gcc";
    }
    return comp ? true : false;
}