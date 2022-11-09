#include "binHeaders.h"
char command[100]; // input() puts into command

void findfile(char *str) {
  input("sudo find / -iname '");
  input(str);
  input("'");
  system(command);
};

int main(int argc, char **argv) {
  if (argc > 1) {
    goto arg;
  }
  printf("No File name entered\nprogram terminated\n");
  exit(0);
arg:
  findfile(*(argv + 1));
  exit(0);
}
