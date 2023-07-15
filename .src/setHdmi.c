#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#ifndef null
#define null NULL
#endif
#ifndef bool
#define bool u_int8_t
#define true 1
#define false 0
#endif
#define err "\e[31m[ ERR ]\e[0m"
#define ok "\e[31m[ OK ]\e[0m"

/* [ file name ] : setHdmi
 * [ file purpose ] : a wrapper xrandr for setting up hdmi
 * [ mode ] : stable
 * [ sample ] : setHdmi
 * [ sample2 ] : setHdmi -m eDP1
 */
/*will add options to pick if mainscreen position relative to others */
/* will add error handling later */

typedef enum {
    small,  // -h
    big,    // --help
} op_size;

typedef struct {
    char* monitor;
    char* size;
    bool show;
} info;

info Io;

char* getbuffer() {
    FILE* fd = popen("xrandr", "r");
    char* buffer = (char*)malloc(1024);
    if (buffer == NULL) {
        fprintf(stderr, "Unable to allocate space for buffer\n");
        exit(1);
    }
    assert(fread(buffer, sizeof(char), 1024, fd));
    pclose(fd);
    return buffer;
}

[[nodiscard("check if the str starts with -")]] bool isNotOption(char* str) {
    if (str != null && str[0] != '-')
        return true;
    return false;
};

/* 1 => exit with err
 * 2 => exit with help message */
void Exit(char* program_name, u_int8_t type) {
    if (type == 1) {
        fprintf(stderr,
                "Invalid arguments passed\ntry %s --help for more information",
                program_name);
        exit(1);
    } else if (type == 2) {
        printf(
            "%s: Used to setup dual displays\n"
            "-h                        print this help message\n"
            "-m [ main monitor name ]  default is eDP1\n"
            "--size                 specify a size[ default: 1920x1080 ]\n"
            "--show                 show the monitors available\n"
            "--off                     turn of HDMI setting\n",
            program_name);
        exit(0);
    }
}

void setup(const int argc, char** argv, int* index, const op_size op) {
    char temp;
    if (op == small)
        temp = argv[*index][1];
    else if (op == big)
        temp = argv[*index][2];

    char* offbuffer = getbuffer();
    switch (temp) {
        case 'm':
            *index += 1;
            if (!isNotOption(argv[*index]))
                Exit(argv[0], 1);

            Io.monitor = argv[*index];

            if (Io.monitor == NULL) {
                printf("Could not read monitor name\n");
                exit(1);
            }
            break;
        case 's':
            if (argv[*index][3] == 'i') {
                *index += 1;
                if (!isNotOption(argv[*index]))
                    Exit(argv[0], 1);

                Io.size = argv[*index];

                if (Io.size == NULL) {
                    printf("Could not read display size\n");
                    exit(1);
                }
            }  // size

            if (argv[*index][3] == 'h') {
                Io.show = true;
            }  // show

            break;
        case 'h':
            Exit(argv[0], 2);
            break;
        case 'o':
            if (strstr(offbuffer,
                       "HDMI1 disconnected (normal left inverted right x "
                       "axis y axis)") != NULL) {
                fprintf(stderr, err
                        " no HDMI setup found\nTry running"
                        "\"xrandr --output HDMI1 --off\"\n");
            } else {
                system("xrandr --output HDMI1 --off");
                printf(ok " HDMI settings off\n");
            }
            exit(0);
            break;
        default:
            Exit(argv[0], 1);
            break;
    }
}

int main(int argc, char** argv) {
    Io.monitor = null;
    Io.show = false;
    Io.size = null;

    int i = {1};

    // parse argv
    while (i < argc) {
        int first_dash = {0};
        if ('-' == argv[i][0])
            first_dash += 1;
        if (first_dash && '-' == argv[i][1]) {
            setup(argc, argv, &i, big);
        } else if (first_dash) {
            setup(argc, argv, &i, small);
        }
        i++;
    }

    char* buffer = getbuffer();
    if (Io.show)
        printf("======= buffer =======\n%s======= buffer =======\n", buffer);

    Io.monitor = (Io.monitor == NULL ? (char*)"eDP1" : Io.monitor);

    Io.size = (Io.size == NULL ? (char*)"1920x1080" : Io.size);
    if (strstr(buffer, Io.monitor) != NULL) {
        fprintf(stdout, "%s Main Display found \n", ok);
    } else {
        fprintf(stderr,
                "%s Main Display not found\nrun with -m [monitor name]\n", err);
        exit(1);
    }

    if (strstr(buffer, "HDMI1 connected") != NULL) {
        printf("%s HDMI connection found \n", ok);
        memset(buffer, 0, strlen(buffer));
        sprintf(buffer,
                "xrandr --output %s --primary --mode 1920x1080 --rotate normal "
                "--output HDMI1 --mode %s --rotate normal --right-of %s",
                Io.monitor, Io.size, Io.monitor);
        system(buffer);
        printf(ok " Hdmi has been setup sucessfully \n");
        if (argc == 1) {
            printf("command:\n%s\n", buffer);
            printf(ok " %s Right HDMI1 left\n", Io.monitor);
        }
        free(buffer);
        exit(0);
    } else {
        free(buffer);
        fprintf(stderr, err " Could not find HDMI connection \n");
        exit(1);
    }
}
