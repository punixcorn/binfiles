#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char command[100]; // input puts into command

void runasm(char *str) {
  sprintf(command, "gcc  %s -o main -std=c99 -g && ./main ", str);
  system(command);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    runasm("main.c");
  } else if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.c                file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.c file to run\n",
              *(argv));
      exit(0);
    }
    runasm(*(argv + 1));
  } else {
    fprintf(stderr,
            "%s : error, invalid arguments passed\n"
            "try %s -h for help\n",
            *(argv), *(argv));
  }
}
