#include "smallsh.h"
#include "util.h"

/*
 * Run the shell.
 *
 * If the user does not enter the exit command, continue running the shell.
 * Otherwise, exit the loop and terminate the shell.
 */
void runShell(void) {
    const int MAX_LENGTH = 4096;
    char *prompt = ": ";
    char buffer[MAX_LENGTH];
    struct Shell *shell;
    struct Command *command;
    shell = malloc(sizeof(struct Shell));
    initShell(shell, MAX_LENGTH);

    while (*(shell->isRunning)) {
        command = malloc(sizeof(struct Command));
        initCommand(command);
        getUserInput(prompt, buffer, MAX_LENGTH);
        parseCommand(buffer, command);
        if (command->argc != NULL) {
            addNullToCommandVector(command);
            setIsBuiltinCommand(command);
            if (*(command->isBuiltin)) {
                runBuiltinCommand(command, shell);
            } else {
                runExternalCommand(command, shell);
            }
            // printCommand(command);
            // printf("cwd: %s\n", shell->cwd);
        }
        freeCommand(command);
    }
    freeShell(shell);
}

/*
 * Initialize a shell structure.
 *
 * Get the current working directory and the value of the HOME environment
 * variable.
 */
void initShell(struct Shell *shell, int MAX_LENGTH) {
    char *temp = getenv("HOME");
    shell->MAX_LENGTH = malloc(sizeof(int));
    shell->isRunning = malloc(sizeof(int));
    shell->status = malloc(sizeof(int));
    shell->cwd = malloc(sizeof(char) * MAX_LENGTH);
    shell->HOME = malloc(sizeof(char) * MAX_LENGTH);
    *(shell->MAX_LENGTH) = MAX_LENGTH;
    *(shell->isRunning) = 1;
    *(shell->status) = 0;
    shell->cwd = getcwd(shell->cwd, MAX_LENGTH);
    copyString(temp, shell->HOME);
}

/*
 * Free all memory in a shell struct.
 */
void freeShell(struct Shell *shell) {
    free(shell->MAX_LENGTH);
    free(shell->isRunning);
    free(shell->status);
    free(shell->cwd);
    free(shell->HOME);
    free(shell);
}

/*
 * Initialize a command structure with null values.
 */
void initCommand(struct Command *command) {
    command->argc = NULL;
    command->argv = NULL;
    command->isBackground = NULL;
    command->isBuiltin = NULL;
}

/*
 * Print the contents of the command vector for debugging purposes.
 */
void printCommand(struct Command *command) {
    printf("argc: %d\n", *(command->argc));
    for (int i = 0; i < *(command->argc); i++) {
        printf("argv[%d]: %s\n", i, *(command->argv + i));
    }
    if (*(command->isBuiltin)) {
        printf("builtin command: %s\n", *(command->argv));
    }
}

/*
 * Parse user input for commands.
 *
 * Commands are separated by space characters.
 * 
 * If the first character is an octothorpe, interpret the line as a comment.
 * 
 * For each command, call saveCommand() to store it to the command struct.
 */
void parseCommand(char *buffer, struct Command *command) {
    int length = stringLength(buffer);
    int argc = 0;
    int index = 0;
    int count = 0;
    int isCommand = 0;
    char ch = '\0';

    for (int i = 0; i < length; i++) {
        ch = *(buffer + i);

        if (ch == '#' && argc == 0 && count == 0) {
            break;
        }

        if (ch != ' ' && ch != '\t') {
            if (count == 0) {
                index = i;
            }
            if (i == length - 1) {
                isCommand = 1;
            }
            count++;
        } else if (ch == ' ' && count > 0) {
            isCommand = 1;
        }

        if (isCommand) {
            if (ch == '<' && argc > 0 && count == 1) {
                *(command->isStdinRedirection) = 1;
                *(command->stdinFile) = argc + 1;
                break;
            } else if (ch == '>' && argc > 0 && count == 1) {
                *(command->isStdoutRedirection) = 1;
                *(command->stdoutFile) = argc + 1;
                break;
            } else {
                argc++;
                saveCommand(buffer, command, index, count, argc);
            }
            count = 0;
            isCommand = 0;
        }
    }
}

/*
 *
 */
void saveCommand(char *buffer, struct Command *command, int index, int count, int argc) {
    char *str = malloc(sizeof(char) * (count + 1));

    for (int i = 0; i < count; i++) {
        *(str + i) = *(buffer + index + i);
    }

    *(str + count) = '\0';
    
    if (argc == 1) {
        command->isBuiltin = malloc(sizeof(int));
        command->isBackground = malloc(sizeof(int));
        command->isStdinRedirection = malloc(sizeof(int));
        command->isStdoutRedirection = malloc(sizeof(int));
        command->stdinFile = malloc(sizeof(int));
        command->stdoutFile = malloc(sizeof(int));
        command->argc = malloc(sizeof(int));
        command->argv = malloc(sizeof(char*));
        *(command->isStdinRedirection) = 0;
        *(command->isStdoutRedirection) = 0;
    } else {
        command->argv = realloc(command->argv, sizeof(char*) * (argc));
    }

    command->argv = realloc(command->argv, sizeof(char*) * (argc));
    *(command->argc) = argc;
    *(command->argv + argc - 1) = str;
}

/*
 *
 */
void addNullToCommandVector(struct Command *command) {
    *(command->argc) += 1;
    command->argv = realloc(command->argv, sizeof(char*) * (*(command->argc)));
    *(command->argv + *(command->argc) - 1) = NULL;
}

void redirectStdin(struct Command *command) {
    // cat < input.txt
}

void redirectStdout(struct Command *command) {
    // ls > output.txt
}

/*
 * Check if the command is builtin.
 *
 * If the command is builtin, set the respective flag to 1. Otherwise, set it
 * to 0.
 */
void setIsBuiltinCommand(struct Command *command) {
    char *commands[3] = {"exit", "cd", "status"};
    *(command->isBuiltin) = 0;
    for (int i = 0; i < sizeof(commands) / sizeof(char*); i++) {
        if (isEqualString(*(command->argv), *(commands + i))) {
            *(command->isBuiltin) = 1;
            break;
        }
    }
}

/*
 * Check which builtin command was entered.
 *
 * Route the function accordingly.
 */
void runBuiltinCommand(struct Command *command, struct Shell *shell) {
    char *c = *(command->argv);
    if (isEqualString(c, "exit")) {
        runBuiltinCommandExit(command, shell);
    } else if (isEqualString(c, "cd")) {
        runBuiltinCommandCd(command, shell);
    } else if (isEqualString(c, "status")) {
        runBuiltinCommandStatus(command, shell);
    }
}

/*
 * Set the isRunning flag to 0.
 *
 * The shell will terminate accordingly.
 */
void runBuiltinCommandExit(struct Command *command, struct Shell *shell) {
    *(shell->isRunning) = 0;
}

/*
 * Navigate to the specified directory.
 *
 * If argc is equal to 2, only one command was passed, i.e. cd. The second arg
 * is always set equal to NULL. In this case cd home.
 * 
 * Otherwise, if argc is greater than 2, than at least a second arg was passed.
 * Accordingly, attempt to cd into a directory with the value of the second arg.
 */
void runBuiltinCommandCd(struct Command *command, struct Shell *shell) {
    int result = 0;
    if (*(command->argc) == 2) {
        result = chdir(shell->HOME);
    } else {
        result = chdir(*(command->argv + 1));
    }
    if (result == 0) {
        shell->cwd = getcwd(shell->cwd, *(shell->MAX_LENGTH));
    }
}

/*
 * Print the current value of status.
 *
 * The status is equal to the terminating signal of the last foreground process,
 * not including builtin commands.
 */
void runBuiltinCommandStatus(struct Command *command, struct Shell *shell) {
    printf("exit value %d\n", *(shell->status));
}

/*
 *
 */
void runExternalCommand(struct Command *command, struct Shell *shell) {
    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork()");
            break;
        case 0:
            execvp(*(command->argv), command->argv);
            perror("execvp()");
            freeCommand(command);
            freeShell(shell);
            kill(getpid(), SIGKILL);
            break;
        default:
            pid = waitpid(pid, shell->status, 0);
            break;
    }
}

/*
 * Free all memory in a command struct.
 *
 * Iterate through the command vector, freeing all char pointers.
 */
void freeCommand(struct Command *command) {
    if (command->argc != NULL) {
        for (int i = 0; i < *(command->argc); i++) {
            free(*(command->argv + i));
        }
        free(command->isBuiltin);
        free(command->isBackground);
        free(command->isStdinRedirection);
        free(command->isStdoutRedirection);
        free(command->stdinFile);
        free(command->stdoutFile);
        free(command->argc);
        free(command->argv);
    }
    free(command);
}