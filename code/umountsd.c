#include "binHeaders.h"
int main(int argc, char *argv[]) {
  if (argc == 2) {
    char *place = *(argv + 1);
    char command[BUFFERSIZE] = "sudo umount /";
    strcat(command, place);
    system(command);
  } else {
    printf("\nplease enter the directory to umount \neg: /mnt\n");
  }
}
