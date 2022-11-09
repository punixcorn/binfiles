#include "binHeaders.h"
char command[100]; // input() puts into command
int main(int argc, char **argv) {
  input("ping archlinux.org");
  if (argc == 2) {
    goto arg2;
  }
  system(command);
  exit(0);
arg2:
  input(" -c");
  input(*(argv + 1));
  system(command);
  exit(0);
}
