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
        fprintf(stderr, "No 'build.todo' file in the working directory! Run 'todob init'");
        exit(EXIT_FAILURE);
    }

    query = malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (detect()) {
        extern char *comp;
        sprintf(query, "%s", comp);
    } else {
        printf("Neither clang nor gcc not found on your machine!");
        exit(EXIT_FAILURE);
    }

    Token t;

    extern char* path;
    while ((t = get_next(f)) != END) {
        if (get_next(f) != PATH) {
            extern int position;
            fprintf(stderr, "Unexpected token at position: %i", position);
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
            default: {
                fprintf(stderr, "Unexpected token %i", t);
                return;
            }
        }
        sprintf(query, pattern, query, path);
    }
}