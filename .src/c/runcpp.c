#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char command[100]; // input puts into command

void runcpp(char *str) {
  sprintf(command, "g++ %s -o main -g --std=c++14 && ./main", str);
  system(command);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    runcpp("main.cpp");
  } else if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.cpp              file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.cpp file to run\n",
              *(argv));
      exit(0);
    }
    runcpp(*(argv + 1));
  } else {
    fprintf(stderr,
            "%s : error, invalid arguments passed\n"
            "try %s -h for help\n",
            *(argv), *(argv));
  }
}
