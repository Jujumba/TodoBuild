#include "stdio.h"
#include "parser.h"
#include <dirent.h>

void compile();
void create_target();
int main() {
    parse();
    compile();
    return 0;
}

void compile() {
    extern char* query;
    printf("Generated query: %s\n", query);
    create_target();
    chdir("target");
    system(query);
}

void create_target() {
    DIR* target = opendir("target");
    if (!target) {
        system("mkdir target");
    }
    closedir(target);
}