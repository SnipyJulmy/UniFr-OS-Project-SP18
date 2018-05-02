//
// Created by snipy on 05.04.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "../debug.h"

/* Macros */

#define MAKE_AND_SEND do {\
            mkSendBuffer(command);\
            debug("send following command to the server :\n\t%s\n", sendBuff);\
            write(socket_fd, sendBuff, strlen(sendBuff));\
        } while(0);

#define CHECK_ARGC_AND_SEND_COMMAND(NB, COMMAND_NAME) do {\
            if(command->argc != (NB)) \
            {\
                log_warn("Invalid number of argument for : " COMMAND_NAME);\
                return STATUS_FAILURE;\
            }\
        } while(0);\
        MAKE_AND_SEND;

#define CHECK_ARGC_AND_SEND_COMMAND2(A, B, COMMAND_NAME) do {\
            if(command->argc != (A) && command->argc != (B)) \
            {\
                log_warn("Invalid number of argument for : " COMMAND_NAME);\
                return STATUS_FAILURE;\
            }\
        }while(0);\
        MAKE_AND_SEND;


/* Constant */
#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOKEN_DELIMITER " \t\n\r\a"
#define BUFFER_SEND_SIZE 1000

#define STATUS_FAILURE 2
#define STATUS_OK 1
#define STATUS_EXIT 0

// client buffer to send command to the server
char sendBuff[BUFFER_SEND_SIZE];

static char* shell_read_line();
static Command* shell_tokenize_line(char* line);
static int shell_execute(Command* command, int socket_fd);
static void shell_command_destroy(Command* self);
static void mkSendBuffer(Command* command);

static char* fetchTcpLine(int socket_fd);

void shell_loop(int socket_fd, struct sockaddr_in* socket_addr)
{
    char* line;
    Command* command;
    int status;

    do
    {
        // TODO should do this only on help command... not every time
        printf("> ");
        line = shell_read_line();
        command = shell_tokenize_line(line);
        status = shell_execute(command, socket_fd);
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

static int shell_execute(Command* command, int socket_fd)
{
    if (command->argc <= 0)
    {
        debug("No command to execute");
        printf("type help for the usage\n");
        return STATUS_OK;
    }
    if (strcmp(command->args[0], "exit") == 0)
    {
        return STATUS_EXIT;
    }
    else if (strcmp(command->args[0], "help") == 0)
    {
        printf("Usage:\n");
        printf("ls: list table \n add <value>, add <key, value>: add element \n read <key>: read element \n");
        printf("delete <key>: delete element \n update <key, newvalue>: update element \n exit: quit\n");
        return STATUS_OK;
    }
    else if (strcmp(command->args[0], "ls") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND(1, "ls");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else if (strcmp(command->args[0], "add") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND2(2, 3, "add");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else if (strcmp(command->args[0], "read") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND(2, "read");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else if (strcmp(command->args[0], "delete") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND(2, "delete");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else if (strcmp(command->args[0], "update") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND(3, "update");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else if (strcmp(command->args[0], "debug") == 0)
    {
        CHECK_ARGC_AND_SEND_COMMAND(1, "debug");
        char* line = fetchTcpLine(socket_fd);
        printf("%s\n", line);
    }
    else
    {
        log_info("Unknow command : %s\n", command->args[0]);
        log_info("Type help to get the list of available command and usage");
    }

    return STATUS_OK;
}

static char* fetchTcpLine(int socket_fd)
{
    ssize_t n;
    int buffer_size = SHELL_BUFFER_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));

    n = read(socket_fd, buffer, sizeof(char) * (buffer_size - 1));
    if (n <= 0)
        return NULL;
    buffer[n] = 0;
    // TODO check n
    return buffer;
}

// side effect : modify the global variable sendBuff
static void mkSendBuffer(Command* command)
{
    // write the command name inside the buffer with a whitespace
    snprintf(sendBuff, sizeof(sendBuff), "%s", command->args[0]);
    size_t size_used = strlen(command->args[0]);

    for (int i = 1; i < command->argc; i++)
    {
        snprintf(
                &(sendBuff[size_used]),
                sizeof(sendBuff) - size_used,
                " %s",
                command->args[i]
        );
        size_used = size_used + strlen(command->args[i]) + 1;
    }
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
