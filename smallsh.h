#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdio.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 *
 */

struct Shell {
    int *MAX_LENGTH;
    int *isRunning;
    int *status;
    char *cwd;
    char *HOME;
};

struct Command {
    int *isBuiltin;
    int *isBackground;
    int *isStdinRedirection;
    int *isStdoutRedirection;
    int *stdinFile;
    int *stdoutFile;
    int *argc;
    char **argv;
};

void runShell(void);

void initShell(struct Shell *shell, int MAX_LENGTH);

void freeShell(struct Shell *shell);

void initCommand(struct Command *command);

void printCommand(struct Command *command);

void parseCommand(char *buffer, struct Command *command);

void saveCommand(char *buffer, struct Command *command, int index, int count, int argc);

void addNullToCommandVector(struct Command *command);

void redirectStdin(struct Command *command);

void redirectStdout(struct Command *command);

void setIsBuiltinCommand(struct Command *command);

void runBuiltinCommand(struct Command *command, struct Shell *shell);

void runBuiltinCommandExit(struct Command *command, struct Shell *shell);

void runBuiltinCommandCd(struct Command *command, struct Shell *shell);

void runBuiltinCommandStatus(struct Command *command, struct Shell *shell);

void runExternalCommand(struct Command *command, struct Shell *shell);

void freeCommand(struct Command *command);

#endif