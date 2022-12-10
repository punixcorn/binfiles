#include <custom.h>

typedef struct {
  char a[100];
} S;

int main() {
  S *space = (S *)malloc(sizeof(S));
  strncat(space->a, "this is space", 1024);
  printf("%s\n", space->a);
}
