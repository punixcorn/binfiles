#include "binHeaders.h"
int main(int argc, char **argv) {
  if (argc == 2) {
    char *number = *(argv + 1);
    char command[BUFFERSIZE] = "ping archlinux.org -c";
    strcat(command, number);
    system(command);
  } else if (argc == 1) {
    char command[30] = "ping archlinux.org";
    system(command);
  } else {
    printf("unknown arguments, only add the number of pings wanted\neg: cnetwork 4 [ "
           "for 4 pings to the archlinux servers ]");
  }
  exit(0);
}
