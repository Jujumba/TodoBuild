#include "stdio.h"
#include "parser.h"
#include <dirent.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif // _WIN32
bool compile();
void create_target();
void project();
void print_help();
void init_project(char*);
void clean();

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("todob: error: few arguments. Use a '--help' flag to see all available commands\n");
        exit(EXIT_SUCCESS);
    }
    if (!strcmp(argv[1], "--help")) {
        print_help();
    } else if (!strcmp(argv[1], "build")) {
        parse();
        if (!compile()) {
            fprintf(stderr, "todob: error: source files are not valid! Hint: reformat them");
            exit(EXIT_FAILURE);
        }
        project();
        printf("\nBuild files are writen to the target\n");
    } else if (!strcmp(argv[1], "init")) {
        if (argc < 3) {
            fprintf(stderr, "todo: error: too few arguments, specify project name. Hint: run `todob init my_project`\n");
            exit(EXIT_FAILURE);
        }
        init_project(argv[2]);
    } else if (!strcmp(argv[1], "clean")) {
        clean();
    } else {
        printf("todo: error: unknown command. Use a '--help' flag to see all available commands\n");
    }
    return 0;
}

bool compile() {
    extern char* query;
    printf("Generated query: %s\n", query);
    create_target();
    chdir("target");
    bool success = system(query);
    free(query);
    return !success;
}

void create_target() {
    DIR* target = opendir("target");
    if (!target) {
        system("mkdir target");
    }
    closedir(target);
}

void project() {
    extern bool rename_sources;
    extern char *name;
    if (rename_sources) {
        #ifdef _WIN32 
            sprintf(name, "%s.%s", name, "exe");
            remove(name);
            rename("a.exe", name);
        #else
            sprintf(name, "%s.%s", name, "out");
            remove(name);
            rename("a.out", name);
        #endif
        free(name);
    }
}

void print_help() {
    printf("Todo build system.\n\n"); // Seriously, I need to come up with a name
    printf("Commands:\n");
    printf("  init - create a build.todo in the working directory\n");
    printf("  build - build your project following with the rules described in build.todo\n");
    printf("  clear - delete target directory\n");
}

void init_project(char *project) {
    FILE* build_rules = fopen("build.todo", "w");
    char* rule = malloc(sizeof(char) * 35 + strlen(project)); // Magic number <(^.^)>?
    sprintf(rule, "project \"%s\" # Your project starts here...", project);
    fprintf(build_rules, "%s\n", rule);
    free(rule);
}

void clean() {
#ifdef _WIN32
    system("rd /s /q target");
    if (!RemoveDirectory("target")) {
        goto err;
    }
#else
    system("rm -rf target");
    if (rmdir("target")) {
        goto err;
    }
#endif // _WIN32
    return;
    err:
    fprintf(stderr, "todob: error: unable to delete target");
}