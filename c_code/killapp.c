#include "binHeaders.h"
#include <stdlib.h>
#include <string.h>
char command[100];

void killall(char *string) {
  commandis("killall -I -q ");
  commandis(string);
  system(command);
  system(command);
}

int main(int argc, char **argv) {
  if (argc == 2) {
    killall(*(argv + 1));
  } else {
    printf("nothing to kill, please add the process to kill \n\
        eg: killall discord");
  }
  exit(0);
}
