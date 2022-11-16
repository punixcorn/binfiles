#include "binHeaders.h"
char command[100]; // input() puts into command
int main(int argc, char **argv) {
  input("ping archlinux.org");
  if (argc == 2) {
    goto arg2;
  } else if (argc > 2) {
    stderror(
        "cnetwork <number of pings>\t\n\\e[32mexample:\n\t\\e[0m$ cnetwork "
        "4\n\tif no argumets passed it will default to 1000\n");
  }
  system(command);
  exit(0);
arg2:
  input(" -c");
  input(*(argv + 1));
  system(command);
  exit(0);
}
