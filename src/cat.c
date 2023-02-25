// 
// cat.c
// By - Shivoy Arora
// 

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/////////////////////////////// Global variables ///////////////////////////////
#define NUM_WORDS 10            // Max number of word in the input  
#define WORD_LEN 100            // Max number of letters in a word of input

//////////////////////////////////// Colors ////////////////////////////////////
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"
#define BOLD "\033[1m"


///////////////////////////////// Main Function ////////////////////////////////
int main(int argc, char* argv[]) {
    // Formatting input
    if (argc == 1 || argc == 2) {
        printf(RED BOLD "Error: " RESET "Enter name of the file\n");
    }


    char* option = (char*)malloc(sizeof(char) * WORD_LEN);
    char* _paths[NUM_WORDS];
    char* paths[NUM_WORDS];
    int optionChanged = 0;
    int pathIndex = 0;

    for (int i = 0; i < NUM_WORDS; i++) {
        _paths[i] = (char*)malloc(sizeof(char) * WORD_LEN);
        _paths[i][0] = '\0';

        paths[i] = (char*)malloc(sizeof(char) * WORD_LEN);
        paths[i][0] = '\0';
    }
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            optionChanged = 1;
            strcpy(option, "-n");
        }
        else if (strcmp(argv[i], "-E") == 0) {
            optionChanged = 1;
            strcpy(option, "-E");
        }
        else {
            strcpy(_paths[pathIndex], argv[i]);
            pathIndex += 1;
        }
    }

    if (pathIndex == 0) {
        printf(RED BOLD "Error: " RESET "missing operand");
        return 0;
    }
    else {
        for (int i = 0; i < NUM_WORDS && _paths[i][0] != '\0'; i++) {
            if (_paths[i][0] == '/' || _paths[i][0] == '~') {
                strcpy(paths[i], _paths[i]);
            }
            else {
                strcpy(paths[i], argv[1]);
                strcat(paths[i], "/");
                strcat(paths[i], _paths[i]);
            }
        }
    }

    if (optionChanged == 0) {
        strcpy(option, "");
    }

    // Main Program
    int i = 0;
    while (paths[i][0] != '\0') {
        FILE* fptr;
        fptr = fopen(paths[i], "r");
        if (fptr == NULL)
        {
            printf(RED BOLD "Error: " RESET "File %s doesn't exist\n", paths[i]);
            return 0;
        }

        char a;
        int lnNo = 1;
        if (strcmp(option, "-n") == 0) {
            printf("%d\t", lnNo);
        }
        while (feof(fptr) == 0) {
            a = '\0';
            fscanf(fptr, "%c", &a);

            if (a == '\n' && strcmp(option, "-E") == 0) {
                printf("$");
            }

            printf("%c", a);

            if (a == '\n' && strcmp(option, "-n") == 0) {
                lnNo += 1;
                printf("%d\t", lnNo);
            }
        }
        printf("\n\n\n");

        i += 1;


        fclose(fptr);
    }

    free(option);
    return 0;
}