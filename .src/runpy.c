#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*run family
 * [file] : *run
 * [purpose] : look for a main.[format] and run
 * [stable] : stable
 * [sample] : runpy [ runs a main.py ]
 * [sample2] : runpy foo.py [ runs a foo.py file ]
 * */

char command[100]; // input puts into command

void runpy(char *str) {
  sprintf(command, "python3 %s", str);
  system(command);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    runpy("main.py");
  } else if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.py               file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.py file to run\n",
              *(argv));
      exit(0);
    }
    runpy(*(argv + 1));
  } else {
    fprintf(stderr,
            "%s : error, invalid arguments passed\n"
            "try %s -h for help\n",
            *(argv), *(argv));
  }
}
