#include "parser.h"
#include "lexer.h"

extern char* path;
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
            case PROJECT: {
                rename_sources = true;
                name = malloc(sizeof(char) * MAX_PATH_LENGTH);
                strcpy(name, path);
                break;
            }
            default: {
                fprintf(stderr, "Unexpected token %i", t);
                return;
            }
        }
    }
}
