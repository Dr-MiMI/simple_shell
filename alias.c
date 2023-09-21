#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_ARGV_NUM 64

int main(void)
{
    char cmd[MAX_CMD_LEN];
    char *argv[MAX_ARGV_NUM];
    int status;
    char *token;
    int i;

    while (1)
    {
        printf("$ ");
        fgets(cmd, MAX_CMD_LEN, stdin);

        /* Remove trailing newline character */
        cmd[strcspn(cmd, "\n")] = '\0';

        /* Tokenize the command string into an array of arguments */
        i = 0;
        token = strtok(cmd, " ");
        while (token != NULL)
        {
            argv[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (fork() == 0)
        {
            /* Child process */
            if (execve(argv[0], argv, NULL) == -1)
            {
                perror("Error");
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                printf("Command not found\n");
            }
        }
    }

    return (0);
}

