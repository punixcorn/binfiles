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
    break;
  case 2:
    if (strcmp(*(argv + 1), "-h") == 0) {
      printf("%s: Used to setup dual displays\n"
             "-h                      print this help message\n"
             "-m <monitor name>       default is eDP1\n",
             *(argv));
    } else {
      fprintf(stderr,
              "Error: Invaild arguments passed\n\tTry -h for more info\n");
    }
    exit(0);
    break;
  case 3:
    if (strcmp(*(argv + 1), "-m") == 0) {
      monitor = *(argv + 2);
      if (monitor == NULL) {
        printf("Could not read monitor name\n");
        exit(1);
      }
    }
    break;
  default:
    fprintf(stderr, "Invaild arguments passed\n");
    exit(1);
    break;
  }

  FILE *fd = popen("xrandr", "r");
  char *buffer = (char *)malloc(1024 * sizeof(char));
  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate space for buffer\n");
    exit(1);
  }
  // assert(fscanf(fd, "%1023s", buffer));
  assert(fread(buffer, sizeof(char), 1024, fd));
  pclose(fd);

#if debug
  printf("buffer := %s\n", buffer);
#endif
  monitor = monitor == NULL ? "eDP1" : monitor;
  if (strstr(buffer, monitor) != NULL) {
    fprintf(stdout, "Monitor found\n");
  }
  if (strstr(buffer, "HDMI1 connected") != NULL) {
    printf("HDMI connection found\n");
    printf("%s\n%s\n", "eDP1 : primary display is on the left",
           "HDMI1 : secondary display is on the left");
    sprintf(buffer,
            "xrandr --output %s --primary --mode 1920x1080 --rotate normal "
            "--output HDMI1 --mode 1920x1080 --rotate normal --right-of eDP1",
            monitor);
    exit(0);
  } else {
    fprintf(stderr, "%s: %s\n", *argv, "ERR : only one display was read");
    exit(1);
  }
}
