#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD_LEN 256
#define MAX_ARGV_NUM 64

int main(void)
{
    char cmd[MAX_CMD_LEN];
    char *argv[MAX_ARGV_NUM];
    int status;
    int i, j;

    while (1)
    {
        printf("$ ");
        fgets(cmd, MAX_CMD_LEN, stdin);

        /* Remove trailing newline character */
        cmd[strcspn(cmd, "\n")] = '\0';

        /* Tokenize the command string into an array of arguments */
        i = 0;
        j = 0;
        argv[i] = &cmd[0];
        while (cmd[j] != '\0')
        {
            if (cmd[j] == ' ')
            {
                cmd[j] = '\0';
                i++;
                argv[i] = &cmd[j+1];
            }
            j++;
        }
        argv[i+1] = NULL;

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

