#include "binHeaders.h"
char command[100]; // input puts into command
void runcpp(char *string) {
  input("cd $(pwd) && clang++ -o main ");
  input(string);
  input(" && ./main");
  system(command);
  exit(0);
}
int main(int argc, char **argv) {
  if (argc == 1) {
    runcpp("main.cpp");
  } else if (argc == 2) {
    runcpp(*(argv + 1));
  } else {
    stderror(
        "runcpp <file_name>\n\\e[32mexample:\n\t\\e[0m$ runcpp doom.cpp\n\tif "
        "no argument is passed, it will look for 'main.cpp' file to run\n");
  }
}
