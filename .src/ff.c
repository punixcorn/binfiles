#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* ff = findfile
 * a wrapper around find , automatically adds sudo and -iname normal find args
 * can be passed using -p / --params $ ff /home/unix maincargo.cpp --params
 * -printf ...
 */
char command[500], place[2][100], temp[200];

int i = 1;
typedef struct {
  _Bool pTrip;
  _Bool fTrip;
  _Bool cTrip;
  _Bool vTrip;
} Trip;
Trip trip;

void putsinto(char *mainstr, int index, ...);
void sort(const char *str, char **argv, int iter, const int argc);
void run(char *exe_name);

int main(int argc, char **argv) {
  memset(&trip, 0, sizeof(Trip));
  if (argc >= 2) {
    if (strcmp(*(argv + 1), "-h") == 0) {
      fprintf(stdout,
              "Usage : %s [ path ] [ file/directory ] [ parameters... ]\n"
              "-h                     print this message\n"
              "-p --params [args...]  take in normal find parameters\n"
              "-v                     view command before running\n"
              "if a path is not included, it will start from \"/\"\n"
              "path must be an absolute path\n",
              *(argv));
      exit(0);
    }
    goto arg;
  } else if (argc == 1)
    fprintf(stderr,
            "ERR: invalid arguments passed\n"
            "try %s -h for help\n",
            *(argv));
  exit(1);
arg:
  while (i < argc) {
    sort(*(argv + i), argv, i, argc);
    i++;
  }
  run(*(argv));
  exit(0);
}

void putsinto(char *mainstr, int index, ...) {
  va_list ap;
  va_start(ap, index);
  for (int i = 0; i < index; i++) {
    strcat(mainstr, va_arg(ap, char *));
  }
  va_end(ap);
}

void sort(const char *str, char **argv, int iter, const int argc) {
  switch (*(str)) {
  case '-':
    if (strcmp(str, "-p") == 0 || strcmp(str, "--params") == 0) {
      trip.cTrip = true;
      while (iter < argc) {
        iter++;
        if (*(argv + iter) != NULL)
          putsinto(temp, 2, *(argv + iter), " ");
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

void run(char *exe_name) {
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
