// 
// date.c
// By - Shivoy Arora
// 

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>


////////// Main Function //////////
int main(int argc, char* argv[]) {


    char* option = (char*)malloc(sizeof(char) * 100);
    if (argc == 1) {
        strcpy(option, "");
    }
    else {
        strcpy(option, argv[1]);
    }


    if (option[0] == '\0') {
        time_t t = time(NULL);
        printf("%s", ctime(&t));
    }
    else if (strcmp(option, "-u") == 0) {
        time_t now = time(&now);
        struct tm* t = gmtime(&now);
        for (int i = 0; i < 100 && asctime(t)[i] != '\n'; i++) {
            printf("%c", asctime(t)[i]);
        }
        printf(" GMT\n");
    }
    else if (strcmp(option, "-R") == 0) {
        time_t t = time(NULL);

        struct tm lt = { 0 };
        localtime_r(&t, &lt);


        int hr = lt.tm_gmtoff / 3600;
        int min = (lt.tm_gmtoff / 60) % 60;

        for (int i = 0; i < 100 && ctime(&t)[i] != '\n'; i++) {
            printf("%c", ctime(&t)[i]);
        }
        printf(" %02d%d\n", hr, min);

    }

    free(option);
    return 0;
}