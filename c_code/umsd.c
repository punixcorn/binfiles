#include "binHeaders.h"
char command[100];
int main(int argc, char **argv) {
  if (argc == 2) {
    goto argc2;
  }
  stderror("umsd <location>\nexample:\n\tumsd \/mnt\n");
  exit(0);
argc2:
  input("sudo umount /");
  input(*(argv + 1));
  system(command);
  exit(0);
}
