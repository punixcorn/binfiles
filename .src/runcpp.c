#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
char command[100]; // input puts into command
char *cc, *ver, *debug, *flags, *filename;
void runcpp(void) {
  sprintf(command, "%s %s %s %s %s -o main && ./main", cc, filename, ver, debug,
          flags);
  printf("%s\n", command);
  fgetc(stdin);
  system(command);
  exit(0);
}
void Parse(char **buffer, int i) {
  do {
    // if( strncmp("") )
  } while (buffer[i] != NULL);
}
void err(char *name) {
  fprintf(stderr,
          "%s : error, invalid arguments passed\n"
          "try %s -h for help\n",
          name, name);
  exit(1);
}
// -ansi  / -gnu  | -D debug | -F flags extra
int main(int argc, char **argv) {
  if (argc == 1) {
    cc = "g++", ver = "-std=c++17", debug = "", flags = "-g",
    filename = "main.cpp";
    runcpp();
  } else if (argc == 2) {
    if (strncmp(argv[1], "-h", 2) == 0) {
      fprintf(stdout,
              "Useage : %s [ options... ]\n"
              "file.cpp              file to run\n"
              "-h                    print this message\n"
              "[ no arguments  ]     looks for a main.cpp file to run\n",
              *(argv));
      exit(0);
    }
    err(argv[0]);
  } else {
    Parse(argv, argc);
    runcpp();
  }
}
