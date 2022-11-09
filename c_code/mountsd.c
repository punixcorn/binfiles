#include "binHeaders.h"
//$ mountsd sda1 /mnt
char command[100]; // input puts into command
void mountsd(char *str1, char *str2) {
  input("sudo mount /dev/");
  if (*(str1 + 0) == '/') {
    input(str2);
    input(" ");
    input(str1);
  } else {
    input(str1);
    input(" ");
    input(str2);
  }
  system(command);
}

int main(int argc, char **argv) {
  if (argc > 1) {
    goto argc2;
  }
  printf("mount file not given\nmount dirctory not given\n");
  system("lsblk | grep sd");
  exit(0);
argc2:
  mountsd(*(argv + 1), *(argv + 2));
  exit(0);
}
