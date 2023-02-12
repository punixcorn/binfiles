#include "binHeaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char command[100];
int main(int argc, char **argv) {
  sprintf(command, "ping archlinux.org");
  if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout, "Useage : %s [ number of pings ]\n", *(argv));
      exit(0);
    }
    int check = 0;
    if ((check = atoi(*(argv + 1))) == 0) {
      fprintf(stderr,
              "ERR : Invaild values passed\n"
              "Useage : %s [ number of pings ]\n",
              *(argv));
      exit(1);
    }
    goto arg2;
  } else if (argc > 2) {
    fprintf(stderr,
            "ERR : Invaild values passed\n"
            "Useage : %s [ number of pings ]\n",
            *(argv));
    exit(1);
  }
  system(command);
  exit(0);
arg2:
  strcat(command, " -c");
  strcat(command, *(argv + 1));
  system(command);
  exit(0);
}
