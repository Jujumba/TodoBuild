#include "stdio.h"
#include "parser.h"

void compile();

int main() {
    parse();
    compile();
    return 0;
}

void compile() {
    extern char* query;
    printf("Generated query: %s", query);
    system(query);
}