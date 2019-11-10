#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLENGTH 256

int main(int argc, char *argv[]) {
  char *line = NULL;
  char *argument;
  char path[MAXLENGTH] = "/bin/\0";
  size_t len = 0;
  char command[MAXLENGTH] = {0};
  char arguments[MAXLENGTH] = {0};
  char *argv2[10] = {NULL};
  pid_t pid;
  int status;


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
    for (i = 0; i < 10; i++) {
      argv2[i] = NULL;
    }


    if (strcmp(line, "exit\n") == 0) {
      free(line);
      exit(0);
    }

    for (i = 1, str1 = line; ; i++, str1 = NULL) {
      token = strtok_r(str1, "\n", &saveptr1);
      if (token == NULL) {
        break;
      }

      strcpy(path, "\0");
      strcat(path, "/bin/");

      for (str2 = token, j = 0; ;str2 = NULL, j++) {
        subtoken = strtok_r(str2, " ", &saveptr2);

        if (subtoken == NULL) {
          break;
        }

        if (j == 0) {
          //printf("exec %s with arguments: ", subtoken);
          strcpy(command, "\0");
          strcat(command, subtoken);
        } else {
          argv2[j] = malloc(strlen(subtoken) + 1);
          strcpy(argv2[j], subtoken);
        }

        argv2[j] = malloc(strlen(subtoken) + 1);
        strcpy(argv2[j], subtoken);
      }


      strcat(path, command);
      strcat(path, "\0");

      //printf("\nexec %s with arguments:\n", path);
      //
      switch (pid = fork()) {
        case -1: // error in fork
          exit(1);
        case 0: // child process
          if (execv(path, argv2) == -1) {
             perror("execvp");
             exit(1);
          }
          break;
        default: // parent process
           if (wait(&status) == -1) {
              perror("wait");
              exit(1);
           }
        break;
      }

    }

    printf("wish> ");
  }

  return 0;
}
