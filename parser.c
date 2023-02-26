#include "parser.h"
#include "lexer.h"

extern char* path;
char *query = NULL;
void parse() {
    FILE* f = fopen("build.todo" ,"r");
    if (f == NULL) {
        fprintf(stderr, "No file 'build.todo' in the working directory!");
        return;
    }

    struct dirent *entry;
    DIR *dir = opendir(".");

    char *sources[100];
    int head = 0;

    while ((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);
        if (!strcmp(entry->d_name + len - 2, ".c") || !strcmp(entry->d_name + len - 2, ".h")) {
            char* path_p = (char*) malloc(sizeof(char) * MAX_PATH_LENGTH);
            strcpy(path_p, (char*) entry->d_name);
            sources[head++] = path_p;
        }
    }

    if (head == 0) {
        fprintf(stderr, "No source files!");
        return;
    }
    query = malloc(sizeof(char) * MAX_PATH_LENGTH);
    sprintf(query, "clang");

    for (int i = 0; i < head; i++) {
        sprintf(query, "%s ../%s", query, sources[i]); // appending sources
    }

    Token t;

    while ((t = get_next(f)) != END) {
        switch (t) {
            case INCLUDE: {
                if (get_next(f) != PATH) {
                    fprintf(stderr, "Expected: path, but got: bullshit"); // todo: rid of insult
                }
                sprintf(query, "%s -I \"%s\"", query, path);
                break;
            }
            case LINK: {
                if (get_next(f) != PATH) {
                    fprintf(stderr, "Expected: path, but got: bullshit"); // todo: rid of insult
                }
                sprintf(query, "%s -L%s", query, path);
                break;
            }
            case FLAG: {
                if (get_next(f) != PATH) {
                    fprintf(stderr, "Expected: path, but got: bullshit"); // todo: rid of insult
                }
                sprintf(query, "%s %s", query, path);
                break;
            }
            default: {

                fprintf(stderr, "Unexpected token %i", t);
                return;
            }
        }
    }
    for (int i = 0; i < head; ++i) {
        free(sources[i]);
    }
    free(query);
}
