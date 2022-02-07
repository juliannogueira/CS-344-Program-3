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
    int **backgroundPids;
    int *backgroundPidCount;
    int *MAX_LENGTH;
    int *STDIN_FD;
    int *STDOUT_FD;
    int *isRunning;
    int *isRunningBackgroundProcess;
    int *status;
    int *pid;
    char *cwd;
    char *HOME;
};

struct Command {
    int *isBuiltin;
    int *isBackground;
    int *isStdinRedirection;
    int *isStdoutRedirection;
    int *stdinFileArg;
    int *stdoutFileArg;
    int *argc;
    int *wordc;
    char **argv;
    char **wordv;
    FILE *stdinFile;
    FILE *stdoutFile;
};

/*
 * The following functions relate to shell activities.
 */

void runShell(void);

void initShell(struct Shell *shell, int MAX_LENGTH);

void freeShell(struct Shell *shell);

void checkBackgroundPids(struct Shell *shell);

void redirectStdin(struct Command *command, struct Shell *shell);

void redirectStdout(struct Command *command, struct Shell *shell);

void closeFiles(struct Command *command);

void resetOutput(struct Shell *shell);

void setIsBuiltinCommand(struct Command *command);

void setIsBackgroundCommand(struct Command *command);

/*
 * The following functions relate to parsing and assigning commands.
 */

void initCommand(struct Command *command);

void printCommand(struct Command *command);

void parseCommand(char *buffer, struct Command *command, struct Shell *shell);

char *parseExpansion(char *buffer, struct Shell *shell);

void saveCommand(char *buffer, struct Command *command, struct Shell *shell, int index, int count, int wordc, int argc, int isCommand);

void addNullToCommandVector(struct Command *command);

void freeCommand(struct Command *command);

/*
 * The following functions relate to running commands.
 */

void runBuiltinCommand(struct Command *command, struct Shell *shell);

void runBuiltinCommandExit(struct Command *command, struct Shell *shell);

void runBuiltinCommandCd(struct Command *command, struct Shell *shell);

void runBuiltinCommandStatus(struct Command *command, struct Shell *shell);

void runExternalCommandForeground(struct Command *command, struct Shell *shell);

void runExternalCommandBackground(struct Command *command, struct Shell *shell);

#endif