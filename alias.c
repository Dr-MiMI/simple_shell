#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_ALIAS_NUM 64

typedef struct alias {
    char *name;
    char *value;
} alias_t;

alias_t aliases[MAX_ALIAS_NUM];
int alias_count = 0;

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

void handle_alias(char *cmd)
{
    char *name = strtok(&cmd[6], "=");
    int i; // Declare 'i' here

    if (name != NULL)
    {
        char *value = strtok(NULL, "=");
        if (value != NULL)
        {
            /* Define new alias */
            aliases[alias_count].name = strdup(name);
            aliases[alias_count].value = strdup(value);
            alias_count++;
        }
        else
        {
            /* Print existing alias */
            for (i = 0; i < alias_count; i++) // Use 'i' here
            {
                if (strcmp(aliases[i].name, name) == 0)
                {
                    printf("%s='%s'\n", aliases[i].name, aliases[i].value);
                    break;
                }
            }
        }
    }
    else
    {
        /* Print all aliases */
        for (i = 0; i < alias_count; i++) // Use 'i' here
        {
            printf("%s='%s'\n", aliases[i].name, aliases[i].value);
        }
    }
}

// Placeholder for execute_command function - you need to define this function or include its header
int execute_command(char *cmd, char **envp)
{
    // Implement your execute_command logic here
    return 0; // For now, return 0 as a placeholder
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

       /* Check for alias command */
       if (strncmp(cmd, "alias", 5) == 0)
       {
           handle_alias(cmd);
           continue;
       }

       /* Tokenize the command string into separate commands */
       token = strtok(cmd, ";");
       while (token != NULL)
       {
           int status = execute_command(token, envp);
           char *next = strtok(NULL, ";");

           if (next && strchr(next, '&'))
           {
               if (status != 0)
                   break;
           }
           else if (next && strchr(next, '|'))
           {
               if (status == 0)
                   break;
           }

           token = next;
       }
   }

   return (0);
}

