#include "detector.h"

char *comp = NULL;

bool detect() {
    bool found = false;
    printf("Detecting the C compiler...\n");
    if (system("clang --version") == 0) {
        comp = "clang";
        found = true;
    } else if (system("gcc --version") == 0) {
        comp = "gcc";
        found = true;
    }
    return found;
}