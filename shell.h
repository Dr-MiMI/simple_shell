#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_NUM_ARGS 10

typedef struct alias {
    char name[MAX_CMD_LEN];
    char value[MAX_CMD_LEN];
} alias_t;

/* Function to check if a command exists */
int command_exists(char *cmd);

/* Function to split a string into words */
int split(char *str, char **args);

/* Function to execute a command */
void execute_command(char *cmd);

#endif /* SHELL_H */















