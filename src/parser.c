#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

char *query = NULL;
bool rename_sources = false;
char *name = NULL;

void parse() {
    FILE* f = fopen("build.todo", "r");
    if (f == NULL) {
        fprintf(stderr, "todob: error: no 'build.todo' file in the working directory. Hint: 'todob init'");
        exit(EXIT_FAILURE);
    }

    query = malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (detect()) {
        extern char *comp;
        sprintf(query, "%s", comp);
    } else {
        printf("todob: error: neither clang nor gcc not found on your machine's path. Hint: add one");
        exit(EXIT_FAILURE);
    }

    Token t;

    extern char* path;
    while ((t = get_next(f)) != END) {
        if (get_next(f) != PATH) {
            extern int position;
            fprintf(stderr, "todob: error: unexpected token at position: %i. Hint: reformat the build file", position);
            exit(EXIT_FAILURE);
        }
        char *pattern;
        switch (t) {
            case INCLUDE: {
                pattern =  "%s -I \"%s\"";
                break;
            }
            case LINK: {
                pattern =  "%s -L \"%s\"";
                break;
            }
            case FLAG: {
                pattern =  "%s %s";
                break;
            }
            case ADD: {
                pattern =  "%s ../%s";
                break;
            }
            case PROJECT: {
                rename_sources = true;
                name = malloc(sizeof(char) * MAX_PATH_LENGTH);
                strcpy(name, path);
                continue;
            }
        }
        sprintf(query, pattern, query, path);
    }
}