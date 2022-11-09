#include "binHeaders.h"
char command[100];
int main(int argc, char **argv) {
  if (argc == 2) {
    goto argc2;
  }
  printf("no mount directory given\nprogram terminated\n");
  exit(0);
argc2:
  input("sudo umount /");
  input(*(argv + 1));
  system(command);
  exit(0);
}
