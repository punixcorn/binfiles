#include "binHeaders.h"
#include <stdio.h>
char command[100]; // commandis puts into command
void runc(char *string) {
  commandis("cd $(pwd) && gcc -o main ");
  commandis(string);
  commandis(" && ./main");
  system(command);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runc("main.c");
  } else if (argc > 1) {
    runc(*(argv + 1));
  } else {
    printf("either main file not found or file passed as argument not found\n");
  }
  exit(0);
}
