#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* [file name] : mv2
 * [file purpose ]: create a directory if it doesnt exist when moving a file
 * [mode] : stable , not fully tested , debug codes still avaliable
 * [sample]: mv2 --mkdir alacritty.yml ~/.config/alacritty/
 */

char* pname = "mv2";
void info(const char* errmsg, int stat) {
    switch (stat) {
        case 0:
            fprintf(
                stdout,
                "%s: mv command but with extra args...\n"
                "Usage: %s [ options... ] [ file... ] [ place... ]\n"
                "Options:\n"
                "\t--mkdir        create the directory if it doesnt exist\n",
                pname, pname);
            exit(stat);
            break;
        case -1:
            fprintf(stderr, "%s: %s\n", pname, errmsg);
            exit(1);
            break;
        default:
            fprintf(stderr, "%s: Error Occured\n", pname);
            exit(stat);
            break;
    }
}

void createdir(char* dname, mode_t mode) {
    int check = 0;
    DIR* checkdir = opendir(dname);
    if (checkdir == NULL)
        check = mkdir(dname, mode) == 0 ? 0 : 1;

    if (check) {
        info("Error Creating Directory...\n", -1);
    }
}

int main(int argc, char** argv) {
    char* pdir = (char*)malloc(sizeof(char) * 100);
    int check = 0, i = 1, ddebug = {0};
    char* command = malloc(1000);

    if (argc < 2 || (pdir == NULL)) {
        info("Invalid arguments passed", -1);
    }
    if (argc == 2) {
        if (strcmp(*(argv + 1), "-h") == 0)
            info("", 0);
        else
            info("Invalid arguments passed", -1);
    }
    sprintf(command, "mv ");

    while (i < argc) {
        if (strncmp(*(argv + i), "--mkdir", strlen("--mkdir")) == 0) {
            check = 1;
            ++i;
        }
        if (strcmp(*(argv + i), "--debug")) {
            ddebug = 1;
            ++i;
        }
        strcat(command, " ");
        if (*(argv + i) != NULL)
            strcat(command, *(argv + i));
        i++;
    }
    // checking
    if (ddebug) {
        printf("%s\n", command);

        int c = fgetc(stdin);
        if ((char)c == 'q') {
            exit(1);
        }
    }

    if (check) {
        // for the last arguments which is the dir
        // this checks if there the option was placed last
        if (strncmp(*(argv + (argc - 1)), "--mkdir", strlen("--mkdir")) == 0)
            check = argc - 2;
        else
            check = argc - 1;
        char* temp = malloc(100);
        memset(temp, 0, 100);
        // mkdir -p is better
        snprintf(temp, 100, "mkdir -p %s", argv[check]);
        system(temp);
        /*  createdir(*(argv + (check)), 0710); */
    }
    system(command);
    exit(0);
}
