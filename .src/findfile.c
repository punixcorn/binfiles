#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char command[100], place[2][50]; // input() puts into command

int i = 1;
typedef struct {
  _Bool pTrip;
  _Bool fTrip;
} Trip;
Trip trip;

void sort(char *str);
void run(char *exe_name);

int main(int argc, char **argv) {
  if (argc >= 2) {
    if (strcmp(*(argv + 1), "-h") == 0) {
      fprintf(stdout,
              "Usage : %s [ path ] [ file/directory ]\n"
              "if a path is not included, it will start from \"\\\"\n",
              *(argv));
      exit(0);
    }
    goto arg;
  } else if (argc == 1)
    fprintf(stderr,
            "ERR: invalid arguments passed\n"
            "Usage: %s [ options... ]\n",
            *(argv));
  exit(1);
arg:
  while (i < argc) {
    sort(*(argv + i));
    i++;
  }
  run(*(argv));
  exit(0);
}

void sort(char *str) {
  switch (*(str)) {
  case '/':
    trip.pTrip = 1;
    strcat(*(place), str);
    break;
  default:
    trip.fTrip = 1;
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
            "Usage: %s [ options... ]\n",
            exe_name);
    exit(1);
  }
  strcat(command, "' 2> /dev/null");
  system(command);
}
