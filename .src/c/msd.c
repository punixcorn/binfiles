#include "binHeaders.h"
//$ msd sda1 /mnt
char command[100]; // input puts into command

void Perror(void) {
  stderror("msd <mount to dir> <dev name>\n\\e[32mexample:\\e[0m\n\t$ msd /mnt "
           "sda1\n");
}
void mountsd(char *str1, char *str2) {
  input("sudo mount /dev/");
  if (*(str1 + 0) == '/' && *(str2 + 0) != '/') {
    input(str2);
    input(" ");
    input(str1);
  } else if (*(str1 + 0) != '/' && *(str2 + 0) == '/') {
    input(str1);
    input(" ");
    input(str2);
  } else {
    Perror();
  }
  system(command);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    goto argc2;
  }
  Perror();
argc2:
  mountsd(*(argv + 1), *(argv + 2));
  exit(0);
}
