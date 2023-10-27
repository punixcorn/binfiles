#include "defines.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

/* [ file name ] : setHdmi
 * [ file purpose ] : a wrapper xrandr for setting up hdmi
 * [ mode ] : stable
 * [ sample ] : setHdmi
 * [ sample2 ] : setHdmi -m eDP1
 */

/* will add options to pick if mainscreen position relative to others */
/* will add error handling later */

/* arg size */
typedef enum
{
    small, // eg: -h
    big,   // eg: --help
} arg_size;

typedef struct
{
    char *monitor;  // monitor name
    char *Mainsize; // main monitor size
    char *Hdmisize; // hdmi monitor size
    char *place;    // where hdmi monitor should be placed relative to main monitor
    bool show;      // show more information
} info;

/* a global info */
info Io;

/* return what the command 'xrandr' gives */
char *getbuffer()
{
    FILE *fd = popen("xrandr", "r");
    char *buffer = (char *)malloc(1024);
    if (buffer == NULL)
    {
        fprintf(stderr, "Unable to allocate space for buffer\n");
        exit(1);
    }
    assert(fread(buffer, sizeof(char), 1024, fd));
    pclose(fd);
    return buffer;
}

[[nodiscard("true if char * str starts with -")]] bool isNotOption(char *str)
{
    if (str != null && str[0] != '-')
        return true;
    return false;
};

/* 1 => exit with err
   2 => exit with help message
*/

void Exit(char *program_name, u_int8_t type)
{
    if (type == 1)
    {
        fprintf(stderr, "Invalid arguments passed\ntry %s --help for more information", program_name);
        exit(1);
    }
    else if (type == 2)
    {
        printf("%s: Used to setup dual displays\n"
               "--help  -h                      print this help message\n"
               "-m [monitor name]               default is eDP1\n"
               "--Mainsize  -M [size]           specify a Main size  [ default: 1920x1080 ]\n"
               "--Hdmisize  -H [size]           specify an HDMI size [ default: 1920x1080 ]\n"
               "--show  -s                      ouput xrandr\n"
               "--place -p [right/left/up/down] where HDMI monitor should be placed relative to main monitor\n"
               "--off -o                        turn of HDMI Display\n",
               program_name);
        exit(0);
    }
}

void setup(const int argc, char **argv, int *index, const arg_size op)
{
    char temp;
    if (op == small)
        temp = argv[*index][1];
    else if (op == big)
        temp = argv[*index][2];

    char *offbuffer = getbuffer();
    switch (temp)
    {
    case 'm':
        *index += 1;
        if (!isNotOption(argv[*index]))
            Exit(argv[0], 1);

        Io.monitor = argv[*index];

        if (Io.monitor == NULL)
        {
            printf("Could not read monitor name\n");
            exit(1);
        }
        break;
    case 'M':
        *index += 1;
        if (!isNotOption(argv[*index]))
            Exit(argv[0], 1);

        Io.Mainsize = argv[*index];

        if (Io.Mainsize == NULL)
        {
            printf("Could not read display Main size\n");
            exit(1);
        }
        break;
    case 'H':
        *index += 1;
        if (!isNotOption(argv[*index]))
            Exit(argv[0], 1);

        Io.Hdmisize = argv[*index];

        if (Io.Hdmisize == NULL)
        {
            printf("Could not read display HDMI size\n");
            exit(1);
        }
        break;
    case 's':
        if (argv[*index][3] == 'h')
        {
            Io.show = true;
        } // show

        break;
    case 'p':
        *index += 1;
        if (!isNotOption(argv[*index]))
            Exit(argv[0], 1);

        Io.place = argv[*index];
        if (Io.place == NULL)
        {
            printf("Could not read HDMI display position size\n");
            exit(1);
        }

        break;
    case 'h':
        Exit(argv[0], 2);
        break;
    case 'o':
        if (strstr(offbuffer, "HDMI1 disconnected (normal left inverted right x "
                              "axis y axis)") != NULL)
        {
            fprintf(stderr, err " no HDMI setup found\nTry running"
                                "\"xrandr --output HDMI1 --off\"\n");
        }
        else
        {
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

int main(int argc, char **argv)
{
    Io.monitor = null;
    Io.show = false;
    Io.Mainsize = null;
    Io.Hdmisize = null;

    int i = {1};

    // parse argv
    while (i < argc)
    {
        int first_dash = {0};
        if ('-' == argv[i][0])
            first_dash += 1;
        if (first_dash && '-' == argv[i][1])
        {
            setup(argc, argv, &i, big);
        }
        else if (first_dash)
        {
            setup(argc, argv, &i, small);
        }
        i++;
    }

    char *buffer = getbuffer();
    if (Io.show)
        printf("======= buffer =======\n%s======= buffer =======\n", buffer);

    // setting defaults if not specified
    Io.monitor = (Io.monitor == NULL ? (char *)"eDP1" : Io.monitor);
    Io.Mainsize = (Io.Mainsize == NULL ? (char *)"1920x1080" : Io.Mainsize);
    Io.Hdmisize = (Io.Hdmisize == NULL ? (char *)"1920x1080" : Io.Hdmisize);
    // setting up place
    if (Io.place != NULL)
    {
        char tempChar = Io.place[0];
        char *tempstring = Io.place;
        Io.place = nullptr;
        switch (tempChar)
        {
        case 'r':
            Io.place = "--right-of ";
            break;
        case 'l':
            Io.place = "--left-of ";
            break;
        case 'u':
            Io.place = "--above";
            break;
        case 'd':
            Io.place = "--below ";
            break;
        default:
            printf("\e[31m[ WARNING ] Uknown option %s used in --place defaulting to right\e[0m\n", tempstring);
            Io.place = "--right-of ";
            break;
        }
    }
    Io.place = (Io.place == NULL ? (char *)"--right-of" : Io.place);
    if (strstr(buffer, Io.monitor) != NULL)
    {
        fprintf(stdout, "%s Main Display found \n", ok);
    }
    else
    {
        fprintf(stderr, "%s Main Display not found\nrun with -m [monitor name]\n", err);
        exit(1);
    }

    if (strstr(buffer, "HDMI1 connected") != NULL)
    {
        printf("%s HDMI connection found \n", ok);
        memset(buffer, 0, strlen(buffer));
        sprintf(buffer,
                "xrandr --output %s --primary --mode 1920x1080 --rotate normal "
                "--output HDMI1 --mode %s --rotate normal %s %s",
                Io.monitor, Io.Mainsize, Io.place, Io.monitor);
        system(buffer);
        printf(ok " Hdmi has been setup sucessfully \n");
        if (argc == 1)
        {
            printf("command:\n%s\n", buffer);
            printf(ok " %s Right HDMI1 left\n", Io.monitor);
        }
        free(buffer);
        exit(0);
    }
    else
    {
        free(buffer);
        fprintf(stderr, err " Could not find HDMI connection \n");
        exit(1);
    }
}
