#include <stdio.h>

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
    int isRunning = 1;
    char *prompt = ": ";
    char buffer[MAX_LENGTH];
    struct Command *command;

    while (isRunning) {
        command = malloc(sizeof(struct Command));
        initCommand(command);
        getUserInput(prompt, buffer, MAX_LENGTH);
        parseCommand(buffer, command);
        if (command->argc != NULL) {
            addNullToCommandVector(command);
            isBuiltinCommand(command);
            printCommand(command);
        }
        if (isExit(buffer)) {
            isRunning = 0;
        }
        freeCommand(command);
    }
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
            saveCommand(buffer, command, index, count, argc);
            argc++;
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
    
    if (argc == 0) {
        command->argc = malloc(sizeof(int));
        command->argv = malloc(sizeof(char*));
        command->isBuiltin = malloc(sizeof(int));
        command->isBackground = malloc(sizeof(int));
    } else {
        command->argv = realloc(command->argv, sizeof(char*) * (argc + 1));
    }

    command->argv = realloc(command->argv, sizeof(char*) * (argc + 1));
    *(command->argc) = argc + 1;
    *(command->argv + argc) = str;
}

/*
 *
 */
void addNullToCommandVector(struct Command *command) {
    *(command->argc) += 1;
    command->argv = realloc(command->argv, sizeof(char*) * (*(command->argc)));
    *(command->argv + *(command->argc) - 1) = NULL;
}

/*
 * Check if the command is builtin.
 *
 * If the command is builtin, set the respective flag to 1. Otherwise, set it
 * to 0.
 */
void isBuiltinCommand(struct Command *command) {
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
 * Free all memory in a command struct.
 *
 * Iterate through the command vector, freeing all char pointers.
 */
void freeCommand(struct Command *command) {
    if (command->argc != NULL) {
        for (int i = 0; i < *(command->argc); i++) {
            free(*(command->argv + i));
        }
        free(command->argc);
        free(command->argv);
        free(command->isBackground);
        free(command->isBuiltin);
    }
    free(command);
}

/*
 * Check if the exit command was entered.
 *
 * If the exit command was entered, return 1. Otherwise, return 0.
 */
int isExit(char *buffer) {
    char *exit = "exit";
    if (isEqualString(exit, buffer)) {
        return 1;
    } else {
        return 0;
    }
}