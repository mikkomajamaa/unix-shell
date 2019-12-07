#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLENGTH 256

int main(int argc, char *argv[]) {
  char *line = NULL;
  char *path[10] = {"/bin/\0"};
  char temp_path[MAXLENGTH];
  size_t len = 0;
  char command[MAXLENGTH] = {0};
  char *argv2[10] = {NULL};
  pid_t pid;
  int status;

  // error messages
  char no_dir_provided[30] = "No directory provided\n";
  char dir_error[30] = "Error opening directory\n";
  char cmd_not_found[30] = "Command not found\n";
  //

  char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  int i, j, k, path_set;

  // wrong number of arguments
  if (argc > 2) {
    exit(1);
  }

  printf("wish> ");
  while (getline(&line, &len, stdin) != -1) {
    // boolean to check if path command was used
    path_set = 0;

    // initialize argv2
    for (i = 0; i < 10; i++) {
      argv2[i] = NULL;
    }

    // check if "exit" built-in command was given as an input
    if (strcmp(line, "exit\n") == 0) {
      free(line);
      exit(0);
    }

    for (i = 1, str1 = line; ; i++, str1 = NULL) {
      token = strtok_r(str1, "\n", &saveptr1);
      if (token == NULL) {
        break;
      }

      for (str2 = token, j = 0; ;str2 = NULL, j++) {
        subtoken = strtok_r(str2, " ", &saveptr2);

        if (subtoken == NULL) {
          break;
        }

        if (j == 0) {
          strcpy(command, "\0");
          strcat(command, subtoken);
        } else {
          argv2[j] = malloc(strlen(subtoken) + 1);
          strcpy(argv2[j], subtoken);
        }

        argv2[j] = malloc(strlen(subtoken) + 1);
        strcpy(argv2[j], subtoken);
      }

      // check if "path" built-in command was given as an input
      if (strcmp(command, "path") == 0) {
        for (k = 1; k < 10; k++) {
          // no path given
          if (!argv2[k] && k == 1) {
            path[0] = NULL;
          }
          // end of arguments
          if (!argv2[k]) break;
          //
          path[k-1] = malloc(strlen(argv2[k]));
          strcpy(path[k-1], argv2[k]);
        }

        path_set = 1;
      }

      // check if "cd" built-in command was given as an input
      if (strcmp(command, "cd") == 0) {
        // wrong number of arguments to cd
        if (argv2[1] == NULL || argv2[2] != NULL) {
          write(STDERR_FILENO, no_dir_provided, strlen(no_dir_provided));
        } else if (chdir(argv2[1]) == -1) {
          write(STDERR_FILENO, dir_error, strlen(dir_error));
        }
        continue;
      }

      // continue if "path" command was given as an input
      if (path_set) {
        continue;
      }

      switch (pid = fork()) {
        case -1: // error in fork
          exit(1);
        case 0: // child process
          // iterate through path array and generate path
          for (k = 0; k < 10; k++) {
            /* If end of path array is reached it means execv never succeeded i.e.
            execv always returned so child process is still running and needs to
            return explicitly */
            if (!path[k]) {
              write(STDERR_FILENO, cmd_not_found, strlen(cmd_not_found));
              return 0;
            }
            strcpy(temp_path, "\0");
            strcat(temp_path, path[k]);
            strcat(temp_path, command);
            strcat(temp_path, "\0");
            execv(temp_path, argv2);
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
