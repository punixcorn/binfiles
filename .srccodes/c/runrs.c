#include "binHeaders.h"
char command[100]; // input puts into command
void runc(char *string) {
  input("cd $(pwd) && rustc -o main ");
  input(string);
  input(" && ./main");
  system(command);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runc("main.rs");
  } else if (argc > 1) {
    runc(*(argv + 1));
  } else {
    printf("either main file not found or file passed as argument not found\n");
  }
  exit(0);
}
