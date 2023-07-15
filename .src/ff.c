#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* [ file name ] : ff
 * [ file purpose ] : a wrapper around find , automatically adds sudo and -iname
 * normal find args can be passed using -p / --params
 * $ ff /home/unix maincargo.cpp --params -printf ...
 * [ mode ] : stable
 * [ sample ] : ff main.c
 * [ sample2 ] : ff /home/test/ main.c
 * [ sample3 ] : ff /mnt main.c --params [normal find params]
 */

char command[500], place[2][100], temp[200];

int i = 1;
typedef struct {
    bool pTrip;
    bool fTrip;
    bool cTrip;
    bool vTrip;
} Trip;

Trip trip;

void putsinto(char* mainstr, int index, ...);
void parse(const char* str, char** argv, int iter, const int argc);
void run(char* exe_name);

int main(int argc, char** argv) {
    memset(&trip, 0, sizeof(Trip));
    if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            fprintf(stdout,
                    "Usage : %s [ path ] [ file/directory ] [ parameters... ]\n"
                    "-h                     print this message\n"
                    "-p --params [args...]  take in normal find parameters\n"
                    "-v                     view command before running\n"
                    "if a path is not included, it will start from \"/\"\n"
                    "path must be an absolute path\n",
                    argv[0]);
            exit(0);
        }
        goto arg;
    } else if (argc == 1)
        fprintf(stderr,
                "ERR: invalid arguments passed\n"
                "try %s -h for help\n",
                argv[0]);
    exit(1);
arg:
    while (i < argc) {
        parse(*(argv + i), argv, i, argc);
        i++;
    }
    run(*(argv));
    exit(0);
}

void putsinto(char* mainstr, int index, ...) {
    va_list ap;
    va_start(ap, index);
    for (int i = 0; i < index; i++) {
        strcat(mainstr, va_arg(ap, char*));
    }
    va_end(ap);
}

void parse(const char* str, char** argv, int iter, const int argc) {
    switch (*(str)) {
        case '-':
            if (strcmp(str, "-p") == 0 || strcmp(str, "--params") == 0) {
                trip.cTrip = true;
                iter += 1;
                while (iter < argc) {
                    if ((*(argv + iter) != NULL) && (argv[iter][0] != '-')) {
                        putsinto(temp, 2, *(argv + iter), " ");
                        iter++;
                    } else
                        return;
                };
            } else if (strcmp(str, "-v") == 0) {
                trip.vTrip = true;
            }
            break;
        case '/':
            trip.pTrip = 1;
            strcat(*(place), str);
            break;
        default:
            trip.fTrip = 1;
            if (trip.cTrip != true)
                strcat(*(place + 1), str);
            break;
    }
}

void run(char* exe_name) {
    sprintf(command, "sudo find ");
    if (trip.pTrip) {
        strcat(command, *place);
    } else {
        strcat(command, "/");
    }
    strcat(command, " -iname '");
    if (trip.fTrip) {
        strcat(command, *(place + 1));
    } else {
        fprintf(stderr,
                "ERR: invalid arguments passed\n"
                "Usage: %s [ path ] [ file/directory ] [ params...  ]\n",
                exe_name);
        exit(1);
    }
    putsinto(command, 3, "' ", temp, " 2> /dev/null");
    if (trip.vTrip) {
        printf("Command : %s\nPress any key to continue...", command);
        getc(stdin);
    }
    system(command);
    exit(0);
}
