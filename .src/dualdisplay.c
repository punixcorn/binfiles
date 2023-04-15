#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*will add options to pick if mainscreen position relative to others */
/* will add error handling later */
int main(int argc, char **argv) {
  char *monitor = NULL;
  switch (argc) {
  case 1:
    argc = 0;
    break;
  case 2:
    if (strncmp(argv[1], "-h", 2) == 0 ||
        strncmp(argv[1], "--help", strlen("--help")) == 0) {
      printf("%s: Used to setup dual displays\n"
             "-h                      print this help message\n"
             "-m <monitor name>       default is eDP1\n",
             *(argv));
    } else if (strncmp(argv[1], "-s", 2) == 0 ||
               strncmp(argv[1], "--show", strlen("--show")) == 0) {
      argc = 1; // using argc so i dont have to create another variable
      break;
    } else {
      fprintf(stderr,
              "Error: Invaild arguments passed\n\tTry -h for more info\n");
    }
    exit(0);
    break;
  case 3:
    argc = 0;
    if (strcmp(*(argv + 1), "-m") == 0) {
      monitor = *(argv + 2);
      if (monitor == NULL) {
        printf("Could not read monitor name\n");
        exit(1);
      }
    }
    break;
  default:
    fprintf(stderr, "Invalid arguments passed\n");
    exit(1);
    break;
  }
  FILE *fd = popen("xrandr", "r");
  char *buffer = (char *)malloc(1024 * sizeof(char));
  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate space for buffer\n");
    exit(1);
  }
  assert(fread(buffer, sizeof(char), 1024, fd));
  pclose(fd);

  if (argc == 1)
    printf("======= buffer =======\n%s======= buffer =======\n", buffer);
  monitor = (monitor == NULL ? (char *)"eDP1" : monitor);
  if (strstr(buffer, monitor) != NULL) {
    fprintf(stdout, "Main Monitor found\n");
  }
  if (strstr(buffer, "HDMI1 connected") != NULL) {
    printf("HDMI connection found\n");
    printf("%s %s\n", "Placing eDP1 Right", "HDMI1 left");
    sprintf(buffer,
            "xrandr --output %s --primary --mode 1920x1080 --rotate normal "
            "--output HDMI1 --mode 1920x1080 --rotate normal --right-of %s",
            monitor, monitor);
    printf("command:\n%s\n", buffer);
    system(buffer);
    exit(0);
  } else {
    fprintf(stderr, "%s: %s\n", *argv, "ERR :Could not find HDMI connection");
    exit(1);
  }
}
