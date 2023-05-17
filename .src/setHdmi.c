#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define null NULL

/* [ file name ] : setHdmi
 * [ file purpose ] : a wrapper xrandr for setting up hdmi
 * [ mode ] : stable
 * [ sample ] : setHdmi
 * [ sample2 ] : setHdmi -m eDP1
 */
/*will add options to pick if mainscreen position relative to others */
/* will add error handling later */

char *getbuffer() {
  FILE *fd = popen("xrandr", "r");
  char *buffer = (char *)malloc(1024);
  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate space for buffer\n");
    exit(1);
  }
  assert(fread(buffer, sizeof(char), 1024, fd));
  pclose(fd);
  return buffer;
}

int main(int argc, char **argv) {

  char *monitor = null;
  char *size = "1920x1080";
  char *err = "\e[31m[ ERR ]\e[0m";
  char *ok = "\e[31m[ OK ]\e[0m";

  switch (argc) {
  case 1:
    argc = 0; // using argc , cus free varibale to use
    break;
  case 2:
    if (strncmp(argv[1], "-h", 2) == 0 ||
        strncmp(argv[1], "--help", strlen("--help")) == 0) {
      printf("%s: Used to setup dual displays\n"
             "-h                        print this help message\n"
             "-m [ main monitor name ]  default is eDP1\n"
             "-s --show                 show the monitors available\n"
             "--off                     turn of HDMI setting\n",
             argv[0]);
    } else if (strncmp(argv[1], "-s", 2) == 0 ||
               strncmp(argv[1], "--show", strlen("--show")) == 0) {
      argc = 1;
      break;
    } else if (strncmp(argv[1], "-off", 3) == 0 ||
               strncmp(argv[1], "--off", strlen("--off")) == 0) {

      char *offbuffer = getbuffer();
      if (strstr(offbuffer, "HDMI1 disconnected (normal left inverted right x "
                            "axis y axis)") != NULL) {

        fprintf(stderr,
                "%s no HDMI setup found\nTry running \"xrandr "
                "--output HDMI1 --off\"\n",
                err);
      } else {
        system("xrandr --output HDMI1 --off");
        printf("%s HDMI settings off\n", ok);
      }
      exit(0);
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

  char *buffer = getbuffer();
  if (argc == 1)
    printf("======= buffer =======\n%s======= buffer =======\n", buffer);
  monitor = (monitor == NULL ? (char *)"eDP1" : monitor);

  if (strstr(buffer, monitor) != NULL) {
    fprintf(stdout, "%s Main Display found \n", ok);
  } else {
    fprintf(stderr, "%s Main Display not found\nrun with -m [monitor name]\n",
            err);
    exit(1);
  }

  if (strstr(buffer, "HDMI1 connected") != NULL) {
    printf("%s HDMI connection found \n", ok);
    sprintf(buffer,
            "xrandr --output %s --primary --mode 1920x1080 --rotate normal "
            "--output HDMI1 --mode %s --rotate normal --right-of %s",
            monitor, size, monitor);
    /*
     char *command[15] = {
        "xrandr",    "--output",        monitor,    "--primary",  "--mode",
        "1920x1080", "--rotate normal", "--output", "HDMI1",      "--mode",
        size,        "--rotate",        "normal",   "--right-of", monitor};
    int parent = getpid();
    int status = 0;
    fork();
    if (parent != getpid()) { // we are in child
      status = execv("xrandr", command);
      exit(0);
    }
    if (status == -1) {
      fprintf(stderr, "%s Failed to set Display \n", err);
    } else {
      printf("%s Hdmi has been setup sucessfully \n", ok);
    }*/
    system(buffer);
    printf("%s Hdmi has been setup sucessfully \n", ok);
    if (argc == 1) {
      printf("command:\n%s\n", buffer);
      printf("%s Right HDMI1 left\n", monitor);
    }
    free(buffer);
    exit(0);
  } else {
    free(buffer);
    fprintf(stderr, "%s Could not find HDMI connection \n", err);
    exit(1);
  }
}
