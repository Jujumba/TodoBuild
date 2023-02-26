#include "lexer.h"
#include <stdio.h>
char *path;

Token get_next(FILE* f) {
    static char current = ' ';
    while (isspace(current)) {
        current = (char) getc(f);
    }
    if (current == EOF){
        return END;
    }
    int head = 0;
    if (current == '#') { // skip comments
        while (current != '\n' && current != EOF) {
            current = getc(f);
        }
        current = getc(f);
    }
    if (current == '"') {
        if (path == NULL) {
            path = (char*) malloc(sizeof(char) * MAX_PATH_LENGTH);
        }
        current = (char) getc(f);
        while (current != '"') {
            if (current == EOF) {
                return UNKNOWN; // unclosed "
            }
            path[head++] = current;
            current = (char) getc(f);
        }
        path[head] = '\0';
        current = (char) getc(f);
        return PATH;
    }
    char* command = (char*) malloc(sizeof(char) * MAX_COMMAND_LENGTH);
    int token = -1;
    while (isalpha(current)) {
        if (head > MAX_COMMAND_LENGTH){
            goto ret;
        }
        command[head++] = current;
        current = (char) getc(f);
    }
    command[head] = '\0';
    if (!strcmp(command, INCLUDE_COMMAND)) {
        token =  INCLUDE;
    } else if (!strcmp(command, LINK_COMMAND)) {
        token =  LINK;
    } else if (!strcmp(command, FLAG_COMMAND)) {
        token =  FLAG;
    } else if (!strcmp(command, ADD_COMMAND)) {
        token = ADD;
    }
    ret:
    free(command);
    return token;
}

