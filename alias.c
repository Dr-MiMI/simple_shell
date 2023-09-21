#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256
#define MAX_ARGV_NUM 64

int main(void)
{
    char cmd[MAX_CMD_LEN];
    char *argv[MAX_ARGV_NUM];
    int status;
    char *token;
    int i;

    struct Alias
aliases[MAX_ARGV_NUM];
    int aliasCount = 0;

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
        argv[i] = NULL;if (argv[0] && argv[1] && strcmp(argv[0], "alias") == 0) {
            char *aliasName = argv[1];
            char *aliasValue = argv[2];

            /* Store the alias*/
            aliases[aliasCount].name = strdup(aliasName);
            aliases[aliasCount].value = strdup(aliasValue);

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
