#include "binHeaders.h"
#include <stdlib.h>
#include <sys/syscall.h>
char command[100], place[2][50]; // input() puts into command
int i = 1;
typedef struct {
  _Bool pTrip;
  _Bool fTrip;
} Trip;
Trip trip;

void sort(char *str);
void run(void);
int main(int argc, char **argv) {
  if (argc >= 2) {
    goto arg;
  } else if (argc == 1)
    stderror("findfile <file/dir> <location>\nexample:\n\tfindfile main.cpp /mnt\n\tif no location passed it will start at \"\/\"\n");
  exit(0);
arg:
  while (i < argc) {
    sort(*(argv + i));
    i++;
  }
  run();
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

void run(void) {
  input("sudo find ");
  if (trip.pTrip) {
    input(*place);
  } else {
    input("/");
  }
  input(" -iname '");
  if (trip.fTrip) {
    input(*(place + 1));
  } else {
    stderror("findfile <dir> <name>\n\\e[32mexample:\n\t\\e[0m$ "
             "findfile /home findfile.c\n"
             "\tif no <dir> is given, it will default to '/'\n");
    exit(0);
  }
  input("'");
  system(command);
}
