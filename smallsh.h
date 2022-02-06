#ifndef SMALLSH_H
#define SMALLSH_H

/*
 *
 */

struct Command {
    int *isBuiltin;
    int *isBackground;
    int *argc;
    char **argv;
};

void runShell(void);

void initCommand(struct Command *command);

void printCommand(struct Command *command);

void parseCommand(char *buffer, struct Command *command);

void saveCommand(char *buffer, struct Command *command, int index, int count, int argc);

void addNullToCommandVector(struct Command *command);

void isBuiltinCommand(struct Command *command);

void freeCommand(struct Command *command);

int isExit(char *buffer);

#endif