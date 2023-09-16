#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_NUM_ARGS 10

/* Function to check if a command exists */
int command_exists(char *cmd)
{
    char *path = getenv("PATH");
    char *p = strtok(path, ":");
    char full_path[MAX_CMD_LEN];

    while (p != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", p, cmd);
        if (access(full_path, F_OK) == 0)
        {
            return 1;
        }
        p = strtok(NULL, ":");
    }

    return 0;
}

/* Function to split a string into words */
int split(char *str, char **args)
{
    int i = 0;
    while (*str != '\0')
    {
        while (isspace(*str))
        {
            *str++ = '\0';
        }
        if (*str != '\0')
        {
            args[i++] = str;
        }
        while (*str != '\0' && !isspace(*str))
        {
            str++;
        }
    }
    args[i] = NULL;
    return i;
}

void execute_command(char *cmd)
{
    char *argv[MAX_NUM_ARGS];
    int status;
    pid_t pid;

    /* Split the command string into arguments */
    int argc = split(cmd, argv);

    /* Check for 'exit' command */
    if (strcmp(argv[0], "exit") == 0)
    {
        if (argc > 1)
        {
            exit(atoi(argv[1]));
        }
        else
        {
            exit(0);
        }
    }

    /* Check if the command exists before forking */
    if (!command_exists(argv[0]))
    {
        fprintf(stderr, "%s: command not found\n", argv[0]);
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execvp(argv[0], argv) == -1)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv)
{
     char line[MAX_CMD_LEN];
     int last_status = 0;

     FILE *fp = stdin;
     if (argc > 1)
     {
         fp = fopen(argv[1], "r");
         if (!fp)
         {
             perror("Error opening file");
             return EXIT_FAILURE;
         }
     }

     while (fgets(line, MAX_CMD_LEN, fp))
     {
         /* Remove trailing newline character */
         line[strcspn(line, "\n")] = 0;

         /* Replace $$ with shell PID and $? with last status */
         char expanded_line[MAX_CMD_LEN] = "";
         for (int i = 0; line[i] != '\0'; i++)
         {
             if (line[i] == '$')
             {
                 if (line[i+1] == '$')
                 {
                     char pid_str[10];
                     sprintf(pid_str, "%d", getpid());
                     strcat(expanded_line, pid_str);
                     i++;
                 }
                 else if (line[i+1] == '?')
                 {
                     char status_str[10];
                     sprintf(status_str, "%d", last_status);
                     strcat(expanded_line, status_str);
                     i++;
                 }
                 else
                 {
                     strncat(expanded_line, &line[i], 1);
                 }
             }
             else
             {
                 strncat(expanded_line, &line[i], 1);
             }
         }

         /* Split the line into commands */
         char *cmd = strtok(expanded_line, ";");
         while (cmd != NULL)
         {
             /* Ignore comments */
             char *comment_start = strchr(cmd, '#');
             if (comment_start)
             {
                 *comment_start = '\0';
             }

             execute_command(cmd);
             cmd = strtok(NULL, ";");
         }
     }

     if (fp != stdin)
     {
         fclose(fp);
     }

     return EXIT_SUCCESS;
}

