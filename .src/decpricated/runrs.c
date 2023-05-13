#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*run family
 * [file] : *run
 * [purpose] : look for a main.[format] and run
 * [stable] : stable
 * [sample] : runcpp [ runs a main.cpp ]
 * [sample2] : runcpp foo.cpp [ runs a foo.cpp file ]
 * */

char command[100]; // input puts into command

void runrs(char *str) {
  sprintf(command, "rustc %s -o main && ./main ", str);
  system(command);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    runrs("main.rs");
  } else if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.rs               file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.rs file to run\n",
              *(argv));
      exit(0);
    }
    runrs(*(argv + 1));
  } else {
    fprintf(stderr,
            "%s : error, invalid arguments passed\n"
            "try %s -h for help\n",
            *(argv), *(argv));
  }
}
