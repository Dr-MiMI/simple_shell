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

int main(void)
{
    char cmd[MAX_CMD_LEN];
    char *argv[MAX_ARGV_NUM];
    int status;
    char *token;
    int i;
    char *fullpath;

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

        fullpath = search_path(argv[0]);
        
        if (fullpath == NULL)
        {
            printf("Command not found\n");
            continue;
        }

        if (fork() == 0)
        {
            /* Child process */
            if (execve(fullpath, argv, NULL) == -1)
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
        
        free(fullpath);
    }

    return (0);
}

