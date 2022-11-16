#include "binHeaders.h"
#include <stdio.h>
char command[100]; // input puts into command
void runc(char *string) {
  input("cd $(pwd) && gcc -o main ");
  input(string);
  input(" && ./main");
  system(command);
  exit(0);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runc("main.c");
  } else if (argc == 2) {
    runc(*(argv + 1));
  } else {
    stderror("runc <file_name>\n\\e[32mexample:\n\t\\e[0m$ runc doom.c\n\tif "
             "no argument is passed, it will look for 'main.c' file to run\n");
  }
}
