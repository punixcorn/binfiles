#include <cstdio>
#include <iostream>
#include <new>
#include <string.h>
#include <string>
int main(int argc, char **argv) {
  // "svn export <git repo>"
  // replace "/tree/main" = trunk
  if (argc < 2)
    exit(1);
  char str[100];
  strcat(str, "svn export ");
  while (sscanf(*(argv + 1), "%s") != EOF)
    ;
  if (*(argv + 1) != NULL && atoi(*(argv + 1)) == 0)
    strcat(str, *(argv + 1));

  exit(0);
}
