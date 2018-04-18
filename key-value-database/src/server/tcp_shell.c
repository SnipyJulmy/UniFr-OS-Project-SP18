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

#define STATUS_FAILURE 2
#define STATUS_OK 1
#define STATUS_EXIT 0

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
        line = tcp_shell_read_line(connectionArgs);
        if (line == NULL)
            goto end;
        command = tcp_shell_tokenize_line(line);
        status = tcp_shell_execute(command);

        free(line);
        command->destroy(command);
    } while (status != STATUS_EXIT);
    end:;
}

Command* tcp_shell_command_create(char** args, int argc)
{
    Command* command = malloc(1 * sizeof(Command));
    command->args = args;
    command->argc = argc;
    command->destroy = tcp_shell_command_destroy;
    check_mem_and_exit(command);
    return command;
}

static void tcp_shell_command_destroy(Command* self)
{
    free(self->args);
    free(self);
}

static int tcp_shell_execute(Command* command)
{
    if (command->argc <= 0)
        return STATUS_EXIT;

    // TODO
    if (strcmp(command->args[0], "ls") == 0)
    {
        // TODO ls
    }

    log_info("Shell execute command :\n");
    log_info_mul(
            for (int i = 0; i < command->argc; i++)
            {
                log_info_print("%s ", command->args[i]);
            }
    );
    log_info_nl;
    return STATUS_OK;
}

static Command* tcp_shell_tokenize_line(char* line)
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
    return tcp_shell_command_create(tokens, position);
}

// read a line from the command line interface
static char* tcp_shell_read_line(ServerConnectionArgs* connectionArgs)
{
    ssize_t n;
    int buffer_size = SHELL_BUFFER_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));

    n = read(connectionArgs->connfd, buffer, sizeof(char) * (buffer_size - 1));
    if (n <= 0)
        return NULL;
    buffer[n] = 0;
    // TODO check n
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
