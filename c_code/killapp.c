#include "binHeaders.h"
char command[100];

void killall(char *string) {
  input("killall -I -q ");
  input(string);
  system(command);
  system(command);
}

int main(int argc, char **argv) {
  if (argc == 2) {
    killall(*(argv + 1));
  } else {
    stderror(
        "killapp <program_name>\n\\e[32mexample:\n\t\\e[0mkillapp Discord\n");
  }
  exit(0);
}
