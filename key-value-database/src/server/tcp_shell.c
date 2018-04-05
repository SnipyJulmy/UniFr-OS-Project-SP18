//
// Created by snipy on 05.04.18.
//

#include <stdlib.h>
#include <unistd.h>

#include "tcp_shell.h"
#include "../debug.h"

#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOKEN_DELIMITER " \t\n\r\a"
#define PROMPT "$> "

static char* tcp_shell_read_line(ServerConnectionArgs* connectionArgs);
static Command* tcp_shell_tokenize_line(char* line);
static int tcp_shell_execute(Command* command);
static void tcp_shell_command_destroy(Command* self);

void tcp_shell_loop(ServerConnectionArgs* connectionArgs)
{
    char* line;
    Command* command;
    int status;

    do
    {
        printf(PROMPT);
        line = tcp_shell_read_line(connectionArgs);
        command = tcp_shell_tokenize_line(line);
        status = tcp_shell_execute(command);

        free(line);
        // command->destroy(command);
    } while (status);
}

Command* shell_command_create(char** args, int argc)
{
    Command* command = malloc(1 * sizeof(Command));
    command->args = args;
    command->argc = argc;
    command->destroy = tcp_shell_command_destroy;
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
    printf("Shell execute command :\n");
    for (int i = 0; i < command->argc; i++)
    {
        printf("%s ", command->args[i]);
    }
    printf("\n");
    return 0;
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
static char* shell_read_line(ServerConnectionArgs* connectionArgs)
{
    ssize_t n;
    int buffer_size = SHELL_BUFFER_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));

    while ((n = read(connectionArgs->connfd, buffer, sizeof(char) * (buffer_size - 1))) > 0)
    {
        buffer[n] = 0;
    }
    return buffer;
}


/*
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
 */
