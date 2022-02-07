#include "util.h"

/*
 * Get the size of a null-terminated character array.
 */
int stringLength(char *str) {
    int i = 0;
    while (*(str + i) != '\0') {
        i++;
    }
    return i;
}

/*
 * Copy the contents of source into dest.
 */
void copyString(char *source, char *dest) {
    int length = stringLength(source);
    for (int i = 0; i < length; i++) {
        *(dest + i) = *(source + i);
    }
    *(dest + length) = '\0'; 
}

/*
 * Compare two null-terminated character arrays.
 *
 * If the character arrays contain the same contents, return 1. Otherwise,
 * return 0.
 */
int isEqualString(char *str1, char *str2) {
    int length = stringLength(str1);
    if (length != stringLength(str2)) {
        return 0;
    }
    for (int i = 0; i < length; i++) {
        if (*(str1 + i) != *(str2 + i)) {
            return 0;
        }
    }
    return 1;
}

/*
 * Convert an integer to a null-terminated character array.
 *
 * Return the dynamically allocated array.
 * 
 * The calling function must free memory accordingly.
 */
char *integerToString(int num) {
    int index = 0;
    int isNegative = 0;
    char temp = 0;
    char *string = malloc(sizeof(char) * 1);

    if (num < 0) {
        isNegative = 1;
        num *= -1;
    }

    while (num != 0) {
        index++;
        string = realloc(string, sizeof(char) * index);
        *(string + index - 1) = (num % 10) + 48;
        num /= 10;
    }

    if (isNegative) {
        index++;
        string = realloc(string, sizeof(char) * index);
        *(string + index - 1) = '-';
    }

    for (int i = 0; i < index / 2; i++) {
        temp = *(string + i);
        *(string + i) = *(string + index - 1 - i);
        *(string + index - 1 - i) = temp;
    }

    index++;
    string = realloc(string, sizeof(char) * index);
    *(string + index - 1) = '\0';

    return string;
}

/*
 * Prompt the user for input.
 *
 * Display the prompt, then get size characters from the user.
 * 
 * The newline character or last character, whichever comes first, will be
 * converted to a null-terminator.
 */
void getUserInput(char *prompt, char *buffer, int size) {
    printf("%s", prompt);

    fflush(stdout);
    
    fgets(buffer, size, stdin);

    for (int i = 0; i < size; i++) {
        if (*(buffer + i) == '\n') {
            *(buffer + i) = '\0';
            break;
        }
    }
}

/*
 * Parse a string for a pattern.
 *
 * If the pattern is found, return 1. Otherwise, return 0.
 */
int parseString(char *pattern, char *string) {
    for (int i = 0; i < stringLength(string); i++) {
        if (*(pattern + 0) == *(string + i)) {
            for (int j = 0; j < stringLength(pattern); j++) {
                if ((i + stringLength(pattern)) > stringLength(string)) {
                    break;
                } else if (*(pattern + j) != *(string + i + j)) {
                    break;
                } else if (j == (stringLength(pattern) - 1)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*
 * Check if the passed file can be opened.
 */
int isValidFile(char *filename, char *mode) {
    FILE *fp;
    fp = fopen(filename, mode);
    if (fp != NULL) {
        fclose(fp);
        return 1;
    } else {
        return 0;
    }
}