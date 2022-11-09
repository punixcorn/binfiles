#include "binHeaders.h"
#include <stdio.h>
char command[100]; // input puts into command
void runasm(char *string) {
  input("cd $(pwd) && nasm -f elf64 ");
  input(string);
  input(" -o main.o && ld main.o -o main && ./main");
  system(command);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runasm("main.asm");
  } else if (argc > 1) {
    runasm(*(argv + 1));
  } else {
    printf("either main file not found or file passed as argument not found\n");
  }
  exit(0);
}
