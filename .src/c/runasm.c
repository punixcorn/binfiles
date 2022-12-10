#include "binHeaders.h"
#include <stdio.h>
#include <stdlib.h>
char command[100]; // input puts into command
void runasm(char *string) {
  input("cd $(pwd) && nasm -f elf64 ");
  input(string);
  input(" -o main.o && ld main.o -o main && ./main");
  system(command);
  exit(0);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runasm("main.asm");
  } else if (argc == 2) {
    runasm(*(argv + 1));
  } else {
    stderror(
        "runasm <file_name>\n\\e[32mexample:\n\t\\e[0m$ runasm doom.asm\n\tif "
        "no argument is passed, it will look for 'main.asm' file to run\n");
  }
}
