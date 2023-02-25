// 
// ls.c
// By - Shivoy Arora
// 

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>


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


    char* path = (char*)malloc(sizeof(char) * 100);
    char* option = (char*)malloc(sizeof(char) * 100);

    if (argc == 1) {
        strcpy(path, ".");
        strcpy(option, "");
    }
    else if (argc == 2) {
        strcpy(path, argv[1]);
        strcpy(option, "");
    }
    else {
        strcpy(path, argv[1]);
        strcpy(option, argv[2]);
    }


    struct dirent** fileListTemp;
    int noOfFiles;

    noOfFiles = scandir(path, &fileListTemp, NULL, alphasort);

    if (noOfFiles < 0) {
        printf(RED BOLD "Error: " RESET "No such file or directory\n");
        return 0;
    }

    if (strcmp(option, "-a") == 0) {
        for (int i = 0; i < noOfFiles; i++) {
            printf("%s\t", fileListTemp[i]->d_name);
        }
    }
    else if (strcmp(option, "-A") == 0) {
        for (int i = 2; i < noOfFiles; i++) {
            printf("%s\t", fileListTemp[i]->d_name);
        }
    }
    else {
        for (int i = 0; i < noOfFiles; i++) {
            if (fileListTemp[i]->d_name[0] != '.') {
                printf("%s\t", fileListTemp[i]->d_name);
            }
        }
    }

    printf("\n");

    free(path);
    free(option);
    free(fileListTemp);
}