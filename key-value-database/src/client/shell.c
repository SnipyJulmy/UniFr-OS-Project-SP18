//
// Created by snipy on 05.04.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shell.h"
#include "../debug.h"

#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOKEN_DELIMITER " \t\n\r\a"

#define STATUS_OK 1
#define STATUS_EXIT 0

static char* shell_read_line();
static Command* shell_tokenize_line(char* line);
static int shell_execute(Command* command);
static void shell_command_destroy(Command* self);

void shell_loop()
{
    char* line;
    Command* command;
    int status;

    do
    {
        printf("> ");
        line = shell_read_line();
        command = shell_tokenize_line(line);
        status = shell_execute(command);

        free(line);
        command->destroy(command);
    } while (status != STATUS_EXIT);
}

Command* shell_command_create(char** args, int argc)
{
    Command* command = malloc(1 * sizeof(Command));
    command->args = args;
    command->argc = argc;
    command->destroy = shell_command_destroy;
    check_mem_and_exit(command);
    return command;
}

static void shell_command_destroy(Command* self)
{
    free(self->args);
    free(self);
}

static int shell_execute(Command* command)
{
    printf("Shell execute command : ");

    if (strcmp(command->args[0], "ls") == 0)
    {
        //Request for list
        printf("ls\n");
    }
    else if (strcmp(command->args[0], "add") == 0)
    {
        //Request for list
        printf("add\n");

    }
    else if (strcmp(command->args[0], "readkey") == 0)
    {
        //Request for list
        printf("readkey\n");

    }
    else if (strcmp(command->args[0], "readval") == 0)
    {
        //Request for list
        printf("readval\n");
    }
    else if (strcmp(command->args[0], "delkey") == 0)
    {
        //Request for list
        printf("delkey\n");

    }
    else if (strcmp(command->args[0], "delval") == 0)
    {
        //Request for list
        printf("delval\n");

    }
    else if (strcmp(command->args[0], "chgkey") == 0)
    {
        //Request for list
        printf("chgkey\n");

    }
    else if (strcmp(command->args[0], "chgval") == 0)
    {
        //Request for list
        printf("chgval\n");

    }
    else if (strcmp(command->args[0], "q") == 0)
    {
        //Request for list
        printf("q\n");
    }

    return STATUS_OK;
}

static Command* shell_tokenize_line(char* line)
{
    int buffer_size = SHELL_BUFFER_SIZE;
    int position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;

    check_mem_and_exit(tokens);
    token = strtok(line, SHELL_TOKEN_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            check_mem_and_exit(tokens);
        }

        token = strtok(NULL, SHELL_TOKEN_DELIMITER);
    }
    tokens[position] = NULL;
    return shell_command_create(tokens, position);
}

// read a line from the command line interface
static char* shell_read_line()
{
    int buffer_size = SHELL_BUFFER_SIZE;
    int position = 0;
    int c;

    char* buffer = malloc(buffer_size * sizeof(char));
    check_mem_and_exit(buffer);

    while (true)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = (char) c;
        }
        position++;

        // double buffer size if we exceed
        if (position >= buffer_size)
        {
            buffer_size = buffer_size + SHELL_BUFFER_SIZE;
            buffer = realloc(buffer, buffer_size * sizeof(char));
            check_mem_and_exit(buffer);
        }
    }
}
