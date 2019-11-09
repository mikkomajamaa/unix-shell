#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *line = NULL;
  size_t len = 0;

  // wrong number of arguments
  if (argc > 2) {
    exit(1);
  }

  printf("wish> ");
  while (getline(&line, &len, stdin) != -1) {
    if (strcmp(line, "exit\n") == 0) {
      free(line);
      exit(0);
    }

    // TODO: handle commands (and arguments)

    //

    printf("wish> ");
  }

  return 0;
}
