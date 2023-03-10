#ifndef BUILDSYS_LEXER_H
#define BUILDSYS_LEXER_H

#define MAX_COMMAND_LENGTH 7
#define MAX_PATH_LENGTH 300
#define INCLUDE_COMMAND "include"
#define LINK_COMMAND "link"
#define FLAG_COMMAND "flag"
#define ADD_COMMAND "add"
#define PROJECT_COMMAND "project"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

typedef enum {
    END = -1,
    PROJECT,
    INCLUDE,
    LINK,
    FLAG,
    PATH,
    ADD,
    UNKNOWN
} Token;


Token get_next(FILE*);

#endif //BUILDSYS_LEXER_H

