#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*will add options to pick if mainscreen position relative to others */
/* will add error handling later */
int main(int argc, char **argv) {
  FILE *fd = popen(
      "xrandr --listactivemonitors | grep \"Monitors:\" | awk '{ print $2 }' ",
      "r");
  char *buffer = (char *)malloc(1024 * sizeof(char));
  while (fscanf(fd, "%1023s", buffer) == 1) {
  }
  int displays = atoi(buffer);
  if (displays == 2) {
    printf("%s\n%s\n", "eDP1 : primary display is on the left",
           "HDMI1 : secondary display is on the left");
    system("xrandr --output eDP1 --primary --mode 1920x1080 --rotate normal "
           "--output HDMI1 --mode 1920x1080 --rotate normal --right-of eDP1");
    exit(0);
  }
  fprintf(stderr, "%s: %s\n", *argv, "ERR : only one display was read");
  exit(1);
}
