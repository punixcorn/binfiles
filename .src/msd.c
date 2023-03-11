#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//$ msd sda1 /mnt
char command[120];
char temp[100]; // input puts into command

void mountsd(char *str1, char *str2, char *exe_name) {
  sprintf(command, "sudo mount /dev/");
  if (*(str1 + 0) == '/' && *(str2 + 0) != '/') {
    sprintf(temp, "%s %s", str2, str1);
  } else if (*(str1 + 0) != '/' && *(str2 + 0) == '/') {
    sprintf(temp, "%s %s", str1, str2);
  } else {
    fprintf(stderr,
            "%s : error, invaild arguments passed...\n"
            "try %s -h for help \n",
            exe_name, exe_name);
    exit(1);
  }
  strncat(command, temp, 99);
  system(command);
}

int main(int argc, char **argv) {
  if (argc == 2) {
    if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'h') {
      fprintf(stdout,
              "Usage : %s [ options... ]\n"
              "[  <device> <directory> ]   mount a device on directory \n"
              "-h                          print this message \n"
              "Example:\n\t%s sda1 /mnt\n",
              *(argv), *(argv));
      exit(0);
    }
    fprintf(stderr,
            "%s : error, invaild arguments passed...\n"
            "Try %s -h for help \n",
            *(argv), *(argv));
    exit(1);
  }
  if (argc == 3) {
    goto argc2;
  }
  fprintf(stderr,
          "%s : error, invaild arguments passed...\n"
          "Try %s -h for help \n",
          *(argv), *(argv));
  exit(1);
argc2:
  mountsd(*(argv + 1), *(argv + 2), *(argv));
  exit(0);
}
