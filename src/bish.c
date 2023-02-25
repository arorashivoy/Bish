// 
// bish.c
// Bullshit Interactive SHell
// By - Shivoy Arora
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <pthread.h>

/////////////////////////////// Global variables ///////////////////////////////
char* username;
char hostname[1024];

#define NUM_WORDS 10            // Max number of word in the input  
#define WORD_LEN 100            // Max number of letters in a word of input

char input[NUM_WORDS][WORD_LEN];


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


//////////////////////////////////// Helper ////////////////////////////////////
void* runThread(void* command) {
    system((char*)command);
    return NULL;
}

void printASCII() {
    printf(RED      "██████╗░██╗░██████╗██╗░░██╗\n" RESET);
    printf(YELLOW   "██╔══██╗██║██╔════╝██║░░██║\n" RESET);
    printf(GREEN    "██████╦╝██║╚█████╗░███████║\n" RESET);
    printf(CYAN     "██╔══██╗██║░╚═══██╗██╔══██║\n" RESET);
    printf(BLUE     "██████╦╝██║██████╔╝██║░░██║\t█▄▄ █▄█   █▀ █░█ █ █░█ █▀█ █▄█\n" RESET);
    printf(PURPLE   "╚═════╝░╚═╝╚═════╝░╚═╝░░╚═╝\t█▄█ ░█░   ▄█ █▀█ █ ▀▄▀ █▄█ ░█░\n" RESET);
}

//////////////////////////////////// Prompt ////////////////////////////////////
void takeInput() {
    // Taking input
    for (int i = 0; i < NUM_WORDS; i++) {
        input[i][0] = '\0';
    }

    char str[NUM_WORDS * WORD_LEN];
    // scanf("%[^\n]s", str);
    // fflush(stdin);
    fgets(str, NUM_WORDS * WORD_LEN, stdin);
    for (int i = 0; i < NUM_WORDS * WORD_LEN != str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
    char* token = strtok(str, " ");
    for (int i = 0; i < NUM_WORDS && token != NULL; i++) {
        strcpy(input[i], token);
        token = strtok(NULL, " ");
    }

    // for (int i = 0; i < NUM_WORDS && input[i][0] != '\0'; i++) {
    //     printf("%s\t", input[i]);
    // }

}

void prompt() {
    // username
    printf(CYAN BOLD "%s" RESET, username);
    printf("@");
    printf(BLUE "%s " RESET, hostname);

    // current directory
    char tmp[FILENAME_MAX];
    printf(GREEN "%s" RESET, getcwd(tmp, FILENAME_MAX));

    printf(CYAN BOLD"\n█\ue0b0 " RESET);
    takeInput();
}

///////////////////////////////// Main Function ////////////////////////////////
int main(int argc, char** argv) {
    printASCII();
    // Getting the username
    username = getenv("USER");
    hostname[1023] = '\0';
    gethostname(hostname, 1023);


    ///////////////////// Getting the paths of executables /////////////////////
    char tmp[FILENAME_MAX];
    char* absPath = getcwd(tmp, FILENAME_MAX);

    char* arg0 = (char*)malloc(sizeof(char) * FILENAME_MAX);
    strcpy(arg0, argv[0]);
    strcat(arg0, "/");

    if (arg0[0] == '/' || arg0[0] == '~') {
        strcpy(absPath, "");
    }

    char* token = strtok(arg0, "/");
    char* tmpString = (char*)malloc(sizeof(char) * FILENAME_MAX);
    strcpy(tmpString, "");
    while (token != NULL) {
        strcat(absPath, tmpString);
        strcat(absPath, "/");
        strcpy(tmpString, token);
        token = strtok(NULL, "/");
    }

    char* cat = (char*)malloc(sizeof(char) * FILENAME_MAX);
    char* date = (char*)malloc(sizeof(char) * FILENAME_MAX);
    char* ls = (char*)malloc(sizeof(char) * FILENAME_MAX);
    char* mkdir = (char*)malloc(sizeof(char) * FILENAME_MAX);
    char* rm = (char*)malloc(sizeof(char) * FILENAME_MAX);

    strcpy(cat, absPath);
    strcpy(date, absPath);
    strcpy(ls, absPath);
    strcpy(mkdir, absPath);
    strcpy(rm, absPath);

    strcat(cat, "cat");
    strcat(date, "date");
    strcat(ls, "ls");
    strcat(mkdir, "mkdir");
    strcat(rm, "rm");


    ////////////////////////////// Main execution //////////////////////////////
    int run = 1;
    while (run) {
        prompt();

        // Checking the use of threads
        int useThreads = 0;
        if (strcmp(input[0], "&t") == 0) {
            useThreads = 1;
            for (int i = 1; i < NUM_WORDS; i++) {
                strcpy(input[i - 1], input[i]);
            }
            input[NUM_WORDS - 1][0] = '\0';
        }

        ////////////////////////// Internal Commands ///////////////////////////
        // exit
        if (strcmp(input[0], "exit") == 0) {
            run = 0;
            return 0;
        }

        // help
        else if (strcmp(input[0], "help") == 0) {
            printf("To get the help regarding the commands of the shell refer to README.md\n");
        }

        // cd
        else if (strcmp(input[0], "cd") == 0) {
            if (strcmp(input[1], "-P") == 0) {
                if (chdir(input[1]) != 0) {
                    printf(RED BOLD "Error: " RESET);
                    printf("no such file or directory\n");
                }
            }
            else {
                if (chdir(input[1]) != 0) {
                    printf(RED BOLD "Error: " RESET);
                    printf("no such file or directory\n");
                }
            }
        }

        // pwd
        else if (strcmp(input[0], "pwd") == 0) {
            int flag = 1;
            for (int i = 1; i < NUM_WORDS && input[i][0] != '\0'; i++) {
                if (strcmp(input[1], "-P") == 0) {
                    flag = 0;
                }
                else if (strcmp(input[1], "-L") == 0) {
                    flag = 1;
                }
                else {
                    printf(RED BOLD "Error: " RESET);
                    printf("Command-line option not recognised\n");
                    continue;
                }
            }

            if (flag) {
                char buff[FILENAME_MAX];
                printf("%s\n", getcwd(buff, FILENAME_MAX));
            }
            else {
                char buff[FILENAME_MAX];
                printf("%s\n", getcwd(buff, FILENAME_MAX));
            }
        }

        // echo
        else if (strcmp(input[0], "echo") == 0) {
            int flag = 1;
            for (int i = 1; i < NUM_WORDS && input[i][0] != '\0'; i++) {
                if (strcmp(input[i], "-E") == 0) {
                    flag = 1;
                }
                else if (strcmp(input[i], "-n") == 0) {
                    flag = 0;
                }
                else {
                    printf("%s ", input[i]);
                }
            }

            if (flag) {
                printf("\n");
            }
        }
        ////////////////////////// External Commands ///////////////////////////
        // ls
        else if (strcmp(input[0], "ls") == 0) {
            char* option = (char*)malloc(sizeof(char) * WORD_LEN);
            char* _path = (char*)malloc(sizeof(char) * WORD_LEN);
            char* path = (char*)malloc(sizeof(char) * WORD_LEN);
            int optionChanged = 0;
            int pathChanged = 0;
            for (int i = 1; i < NUM_WORDS && input[i][0] != '\0'; i++) {
                if (strcmp(input[i], "-A") == 0) {
                    optionChanged = 1;
                    strcpy(option, "-A");
                }
                else if (strcmp(input[i], "-a") == 0) {
                    optionChanged = 1;
                    strcpy(option, "-a");
                }
                else {
                    pathChanged = 1;
                    strcpy(_path, input[i]);
                }
            }

            if (pathChanged == 0) {
                char tmp[FILENAME_MAX];
                char* currPath = getcwd(tmp, FILENAME_MAX);
                strcpy(path, currPath);
            }
            else {
                if (_path[0] == '/') {
                    strcpy(path, _path);
                    strcat(path, "/");
                }
                else if (_path[0] == '~') {
                    strcpy(path, _path);
                }
                else {
                    char tmp[FILENAME_MAX];
                    char* currPath = getcwd(tmp, FILENAME_MAX);
                    strcpy(path, currPath);
                    strcat(path, "/");
                    strcat(path, _path);
                }
            }

            if (optionChanged == 0) {
                strcpy(option, "");
            }

            // Threads
            if (useThreads == 1) {
                pthread_t thread_id;
                char* command = (char*)malloc(sizeof(char) * 2048);

                strcpy(command, ls);
                strcat(command, " ");
                strcat(command, path);
                strcat(command, " ");
                strcat(command, option);

                pthread_create(&thread_id, NULL, runThread, command);
                pthread_join(thread_id, NULL);
            }
            // FORK Process
            else {
                if (fork() == 0) {
                    execl(ls, ls, path, option, NULL);
                    break;
                }
                else {
                    wait(NULL);
                }
            }
        }

        // date
        else if (strcmp(input[0], "date") == 0) {
            char* option = (char*)malloc(sizeof(char) * WORD_LEN);
            int optionChanged = 0;
            for (int i = 1; i < NUM_WORDS && input[i][0] != '\0'; i++) {
                if (strcmp(input[i], "-u") == 0) {
                    strcpy(option, "-u");
                    optionChanged = 1;
                }
                else if (strcmp(input[i], "-R") == 0) {
                    strcpy(option, "-R");
                    optionChanged = 1;
                }
            }

            if (optionChanged == 0) {
                strcpy(option, "");
            }


            // Threads
            if (useThreads == 1) {
                pthread_t thread_id;
                char* command = (char*)malloc(sizeof(char) * 2048);

                strcpy(command, date);
                strcat(command, " ");
                strcat(command, option);

                pthread_create(&thread_id, NULL, runThread, command);
                pthread_join(thread_id, NULL);
            }
            // FORK Process
            else {
                if (fork() == 0) {
                    execl(date, date, option, NULL);
                    break;
                }
                else {
                    wait(NULL);
                }
            }
        }

        // rm
        else if (strcmp(input[0], "rm") == 0) {
            char tmp[FILENAME_MAX];
            char* currPath = getcwd(tmp, FILENAME_MAX);

            // Threads
            if (useThreads == 1) {
                pthread_t thread_id;
                char* command = (char*)malloc(sizeof(char) * 2048);

                strcpy(command, rm);
                strcat(command, " ");
                strcat(command, currPath);
                strcat(command, " ");
                strcat(command, input[1][0] != '\0' ? input[1] : "");
                strcat(command, " ");
                strcat(command, input[2][0] != '\0' ? input[2] : "");
                strcat(command, " ");
                strcat(command, input[3][0] != '\0' ? input[3] : "");
                strcat(command, " ");
                strcat(command, input[4][0] != '\0' ? input[4] : "");
                strcat(command, " ");
                strcat(command, input[5][0] != '\0' ? input[5] : "");
                strcat(command, " ");
                strcat(command, input[6][0] != '\0' ? input[6] : "");
                strcat(command, " ");
                strcat(command, input[7][0] != '\0' ? input[7] : "");
                strcat(command, " ");
                strcat(command, input[8][0] != '\0' ? input[8] : "");
                strcat(command, " ");
                strcat(command, input[9][0] != '\0' ? input[9] : "");


                pthread_create(&thread_id, NULL, runThread, command);
                pthread_join(thread_id, NULL);
            }
            // FORK Process
            else {
                if (fork() == 0) {
                    execl(rm, rm, currPath, input[1][0] != '\0' ? input[1] : "", input[2][0] != '\0' ? input[2] : "", input[3][0] != '\0' ? input[3] : "", input[4][0] != '\0' ? input[4] : "", input[5][0] != '\0' ? input[5] : "", input[6][0] != '\0' ? input[6] : "", input[7][0] != '\0' ? input[7] : "", input[8][0] != '\0' ? input[8] : "", input[9][0] != '\0' ? input[9] : "", NULL);
                    break;
                }
                else {
                    wait(NULL);
                }
            }
        }

        // cat
        else if (strcmp(input[0], "cat") == 0) {
            char tmp[FILENAME_MAX];
            char* currPath = getcwd(tmp, FILENAME_MAX);


            // Threads
            if (useThreads == 1) {
                pthread_t thread_id;
                char* command = (char*)malloc(sizeof(char) * 2048);

                strcpy(command, cat);
                strcat(command, " ");
                strcat(command, currPath);
                strcat(command, " ");
                strcat(command, input[1][0] != '\0' ? input[1] : "");
                strcat(command, " ");
                strcat(command, input[2][0] != '\0' ? input[2] : "");
                strcat(command, " ");
                strcat(command, input[3][0] != '\0' ? input[3] : "");
                strcat(command, " ");
                strcat(command, input[4][0] != '\0' ? input[4] : "");
                strcat(command, " ");
                strcat(command, input[5][0] != '\0' ? input[5] : "");
                strcat(command, " ");
                strcat(command, input[6][0] != '\0' ? input[6] : "");
                strcat(command, " ");
                strcat(command, input[7][0] != '\0' ? input[7] : "");
                strcat(command, " ");
                strcat(command, input[8][0] != '\0' ? input[8] : "");
                strcat(command, " ");
                strcat(command, input[9][0] != '\0' ? input[9] : "");


                pthread_create(&thread_id, NULL, runThread, command);
                pthread_join(thread_id, NULL);
            }
            // FORK Process
            else {
                if (fork() == 0) {
                    execl(cat, cat, currPath, input[1][0] != '\0' ? input[1] : "", input[2][0] != '\0' ? input[2] : "", input[3][0] != '\0' ? input[3] : "", input[4][0] != '\0' ? input[4] : "", input[5][0] != '\0' ? input[5] : "", input[6][0] != '\0' ? input[6] : "", input[7][0] != '\0' ? input[7] : "", input[8][0] != '\0' ? input[8] : "", input[9][0] != '\0' ? input[9] : "", NULL);
                    break;
                }
                else {
                    wait(NULL);
                }
            }
        }

        // mkdir
        else if (strcmp(input[0], "mkdir") == 0) {
            char tmp[FILENAME_MAX];
            char* currPath = getcwd(tmp, FILENAME_MAX);


            // Threads
            if (useThreads == 1) {
                pthread_t thread_id;
                char* command = (char*)malloc(sizeof(char) * 2048);

                strcpy(command, mkdir);
                strcat(command, " ");
                strcat(command, currPath);
                strcat(command, " ");
                strcat(command, input[1][0] != '\0' ? input[1] : "");
                strcat(command, " ");
                strcat(command, input[2][0] != '\0' ? input[2] : "");
                strcat(command, " ");
                strcat(command, input[3][0] != '\0' ? input[3] : "");
                strcat(command, " ");
                strcat(command, input[4][0] != '\0' ? input[4] : "");
                strcat(command, " ");
                strcat(command, input[5][0] != '\0' ? input[5] : "");
                strcat(command, " ");
                strcat(command, input[6][0] != '\0' ? input[6] : "");
                strcat(command, " ");
                strcat(command, input[7][0] != '\0' ? input[7] : "");
                strcat(command, " ");
                strcat(command, input[8][0] != '\0' ? input[8] : "");
                strcat(command, " ");
                strcat(command, input[9][0] != '\0' ? input[9] : "");


                pthread_create(&thread_id, NULL, runThread, command);
                pthread_join(thread_id, NULL);
            }
            // FORK Process
            else {
                if (fork() == 0) {
                    execl(mkdir, mkdir, currPath, input[1][0] != '\0' ? input[1] : "", input[2][0] != '\0' ? input[2] : "", input[3][0] != '\0' ? input[3] : "", input[4][0] != '\0' ? input[4] : "", input[5][0] != '\0' ? input[5] : "", input[6][0] != '\0' ? input[6] : "", input[7][0] != '\0' ? input[7] : "", input[8][0] != '\0' ? input[8] : "", input[9][0] != '\0' ? input[9] : "", NULL);
                    break;
                }
                else {
                    wait(NULL);
                }
            }
        }

        // Unknown command entered
        else {
            printf(RED BOLD "ERROR: " RESET);
            printf("Command not found\n");
        }
    }


    return 0;
}