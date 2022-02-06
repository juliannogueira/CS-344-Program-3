#ifndef UTIL_H
#define UTIL_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/*
 * The util file contains helper functions to be used by smallsh.
 */

int stringLength(char *str);

int isEqualString(char *str1, char *str2);

char *integerToString(int num);

void getUserInput(char *prompt, char *buffer, int size);

int parseString(char *pattern, char *string);

#endif