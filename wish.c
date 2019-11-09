#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *line = NULL;
  char *argument;
  char *path = "/bin";
  size_t len = 0;

  //
  char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  int i, j;

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
    for (i = 1, str1 = line; ; i++, str1 = NULL) {
      token = strtok_r(str1, "\n", &saveptr1);
      if (token == NULL) {
        break;
      }
      //printf("%d: %s\n", i, token);

      for (str2 = token, j = 0; ;str2 = NULL, j++) {
        subtoken = strtok_r(str2, " ", &saveptr2);

        if (subtoken == NULL) {
          break;
        }

        if (j == 0) {
          printf("exec %s with arguments: ", subtoken);
        } else {
          printf("%s ", subtoken);
        }

      }
      printf("\n");
    }

    //

    printf("wish> ");
  }

  return 0;
}
