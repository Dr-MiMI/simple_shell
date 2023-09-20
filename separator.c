#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_ARGV_NUM 64

char *search_path(char *cmd)
{
    char *path = getenv("PATH");
    char *p = strtok(path, ":");
    char *fullpath = malloc(MAX_CMD_LEN);
    
    while (p != NULL)
    {
        snprintf(fullpath, MAX_CMD_LEN, "%s/%s", p, cmd);
        if (access(fullpath, X_OK) == 0)
            return fullpath;
        p = strtok(NULL, ":");
    }
    
    free(fullpath);
    return NULL;
}

void execute_command(char *cmd, char **envp)
{
    char *argv_new[MAX_ARGV_NUM];
    int status;
    char *token;
    int i;
    char *fullpath;

    /* Check for exit command */
    if (strncmp(cmd, "exit", 4) == 0)
    {
        if (cmd[4] == ' ')
        {
            int exit_status = atoi(&cmd[5]);
            exit(exit_status);
        }
        else
        {
            exit(EXIT_SUCCESS);
        }
    }

    /* Check for env command */
    if (strcmp(cmd, "env") == 0)
    {
        for (i = 0; envp[i] != NULL; i++)
            printf("%s\n", envp[i]);
        return;
    }

    /* Check for setenv command */
    if (strncmp(cmd, "setenv", 6) == 0)
    {
        token = strtok(&cmd[7], " ");
        if (token != NULL)
        {
            char *name = token;
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                if (setenv(name, token, 1) != 0)
                {
                    perror("Error");
                }
            }
            else
            {
                fprintf(stderr, "setenv: Syntax error: Missing value\n");
            }
        }
        else
        {
            fprintf(stderr, "setenv: Syntax error: Missing name\n");
        }
        return;
    }

    /* Check for unsetenv command */
    if (strncmp(cmd, "unsetenv", 8) == 0)
    {
        token = strtok(&cmd[9], " ");
        if (token != NULL)
        {
            if (unsetenv(token) != 0)
            {
                perror("Error");
            }
        }
        else
        {
            fprintf(stderr, "unsetenv: Syntax error: Missing name\n");
        }
        return;
    }

    /* Check for cd command */
    if (strncmp(cmd, "cd", 2) == 0)
    {
        char *dir = &cmd[3];
        
        if (*dir == '\0') /* No argument given */
            dir = getenv("HOME");

        if (strcmp(dir, "-") == 0) /* '-' argument given */
            dir = getenv("OLDPWD");

        if (chdir(dir) != 0)
            perror("Error");

         /* Update PWD environment variable */
         setenv("PWD", getcwd(NULL, 0), 1);

         return;
     }

     /* Tokenize the command string into an array of arguments */
     i = 0;
     token = strtok(cmd, " ");
     while (token != NULL)
     {
         argv_new[i] = token;
         i++;
         token = strtok(NULL, " ");
     }
     argv_new[i] = NULL;

     fullpath = search_path(argv_new[0]);
     
     if (fullpath == NULL)
     {
         printf("Command not found\n");
         return;
     }

     if (fork() == 0)
     {
         /* Child process */
         if (execve(fullpath, argv_new, envp) == -1)
         {
             perror("Error");
         }
         exit(EXIT_FAILURE);
     }
     else
     {
         /* Parent process */
         wait(&status);
         
          free(fullpath);
          
          if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
          {
              printf("Command not found\n");
          }
      }        
}

int main(int argc, char **argv, char **envp)
{
   char cmd[MAX_CMD_LEN];
   char *token;

   while (1)
   {
       printf("$ ");
       fgets(cmd, MAX_CMD_LEN, stdin);

       /* Remove trailing newline character */
       cmd[strcspn(cmd, "\n")] = '\0';

       /* Tokenize the command string into separate commands */
       token = strtok(cmd, ";");
       while (token != NULL)
       {
           execute_command(token, envp);
           token = strtok(NULL, ";");
       }
   }

   return (0);
}

