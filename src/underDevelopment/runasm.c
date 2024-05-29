#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char command[100]; // input puts into command

void runasm(char *str) {
  sprintf(command, "nasm -f elf64 %s -o main.o && ld main.o -o main && ./main ",
          str);
  system(command);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    runasm("main.asm");
  } else if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.asm              file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.asm file to run\n",
              argv[0]);
      exit(0);
    }
    runasm(argv[1]);
  } else {
    fprintf(stderr,
            "%s : error, invalid arguments passed\n"
            "try %s -h for help\n",
            argv[0], argv[0]);
  }
}
