#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

extern char* path;
char *query = NULL;
void parse() {
    FILE* f = fopen("build.todo" ,"r");
    if (f == NULL) {
        fprintf(stderr, "No file 'build.todo' in the working directory!");
        return;
    }

    query = malloc(sizeof(char) * MAX_PATH_LENGTH);
    sprintf(query, "clang");

    Token t;

    while ((t = get_next(f)) != END) {
        if (get_next(f) != PATH) {
            extern int position;
            fprintf(stderr, "Unexpected token at position: %i", position);
            exit(EXIT_FAILURE);
        }
        switch (t) {
            case INCLUDE: {
                sprintf(query, "%s -I \"%s\"", query, path);
                break;
            }
            case LINK: {
                sprintf(query, "%s -L%s", query, path);
                break;
            }
            case FLAG: {
                sprintf(query, "%s %s", query, path);
                break;
            }
            case ADD: {
                sprintf(query, "%s ../%s", query, path);
                break;
            }
            default: {
                fprintf(stderr, "Unexpected token %i", t);
                return;
            }
        }
    }
}
