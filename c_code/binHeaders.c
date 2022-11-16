#include "binHeaders.h"
#include <stdlib.h>
char error[BUFFERSIZEX];
void input(char *arg) { strcat(command, arg); }
void stderror(char *arg) {
  strcat(error,
         "printf \"\\e[31mInvalid arguments passed\n\\e[31m***\t\\e[33mprogram "
         "terminated\n\\e[32musage:\n\t\\e[0m");
  strcat(error, arg);
  strcat(error, "\"");
  system(error);
  exit(0);
}
