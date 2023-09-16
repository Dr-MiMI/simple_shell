#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD_LEN 256

int main(void)
{
    char cmd[MAX_CMD_LEN];
    char *argv[2];
    int status;

    while (1)
    {
        printf("$ ");
        fgets(cmd, MAX_CMD_LEN, stdin);

        /* Remove trailing newline character */
        cmd[strcspn(cmd, "\n")] = '\0';

        argv[0] = cmd;
        argv[1] = NULL;

        if (fork() == 0)
        {
            /* Child process */
            if (execve(cmd, argv, NULL) == -1)
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

