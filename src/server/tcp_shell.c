//
// Created by snipy on 05.04.18.
//

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tcp_shell.h"
#include "../debug.h"
#include "../data_structures/dequeue/dequeue.h"
#include "database/database_actions.h"

#define SHELL_BUFFER_SIZE 4096
#define SHELL_TOKEN_DELIMITER " \t\n\r\a"

#define STATUS_FAILURE 2
#define STATUS_OK 1
#define STATUS_EXIT 0

#define COMMAND_EMPTY "__command_empty"
#define COMMAND_ERROR "__command_error"
#define COMMAND_OK "__command_ok"

/* Macros */

// echo on command->args[0]
#define ECHO() \
    do {\
    snprintf(sendBuff, sizeof(sendBuff), "echo %s", command->args[0]); \
    write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
    return STATUS_OK; \
    } while(0);

// -- For argc control

#define CHECK_ARGC(NB, COMMAND_NAME)\
                        do {\
                        log_info("check argc for " COMMAND_NAME); \
                        if(command->argc != (NB)) \
                        { \
                            log_warn("Invalid number of argument for : " COMMAND_NAME);\
                            snprintf(sendBuff, sizeof(sendBuff), COMMAND_ERROR); \
                            write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
                            return STATUS_OK;\
                        }} while(0);

#define CHECK_ARGC_2(A, B, COMMAND_NAME)\
                        do {\
                        log_info("check argc for " COMMAND_NAME); \
                        if(command->argc != (A) && command->argc != (B)) \
                        { \
                            log_warn("Invalid number of argument for : " COMMAND_NAME);\
                            snprintf(sendBuff, sizeof(sendBuff), COMMAND_ERROR); \
                            write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
                            return STATUS_OK;\
                        }} while(0);
/* Macros End */

#define RETURN_COMMAND_ERROR(M, ...) do {\
                        snprintf(sendBuff, sizeof(sendBuff), COMMAND_ERROR " : " M, ##__VA_ARGS__);\
                        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
                        return STATUS_OK;\
                    }while(0);

// server buffer to send back information to the client
char sendBuff[SHELL_BUFFER_SIZE];

static char* tcp_shell_read_line(ServerConnectionArgs* connectionArgs);
static Command* tcp_shell_tokenize_line(char* line);
static int tcp_shell_execute(Command* command, ServerConnectionArgs* connectionArgs);
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
        status = tcp_shell_execute(command, connectionArgs);

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

static int tcp_shell_execute(Command* command, ServerConnectionArgs* connectionArgs)
{
    if (command->argc <= 0)
    {
        debug("No command to execute");
        return STATUS_OK;
    }

    if (strcmp(command->args[0], "ls") == 0) // display the database
    {
        CHECK_ARGC(1, "ls");
        Dequeue* dequeue = database_actions_ls();

        size_t size_used = 0;

        if (!dequeue->is_empty(dequeue))
            while (!dequeue->is_empty(dequeue))
            {
                KV* elt = dequeue->remove_first(dequeue);
                size_used += snprintf(sendBuff + size_used,
                                      sizeof(sendBuff),
                                      "<%u,%s>",
                                      elt->key, elt->value);
            }
        else
            snprintf(sendBuff, sizeof(sendBuff), COMMAND_EMPTY)
                    debug("send following command to the server :\n\t%s\n", sendBuff);
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
        return STATUS_OK;
    }
    else if (strcmp(command->args[0], "add") == 0) // add a <k,v> to the database
    {
        CHECK_ARGC_2(2, 3, "add");

        Key key;

        if (command->argc == 2)
        {
            // allocate memory for the value
            char** value = malloc(1 * sizeof(char*));
            value[0] = malloc(strlen(command->args[1]) * sizeof(char));
            strcpy(value[0], command->args[1]);
            key = database_actions_insert_v(value);
            if (key == 0)
            {
                RETURN_COMMAND_ERROR("unable to insert value <%s>", *value);
            }
            snprintf(sendBuff, sizeof(sendBuff), "key : %u", key);
        }
        else // command->argc == 3
        {
            char** value = malloc(1 * sizeof(char*));
            value[0] = malloc(strlen(command->args[2]) * sizeof(char));
            strcpy(value[0], command->args[2]);
            key = (Key) strtoul(command->args[1], NULL, 10);
            bool status = database_actions_insert_kv(key, value);
            if (!status)
                RETURN_COMMAND_ERROR("unable to add <%u,%s>", key, *value);
            snprintf(sendBuff, sizeof(sendBuff), "key : %u", key);
        }
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
        return STATUS_OK;
    }
    else if (strcmp(command->args[0], "read") == 0) // read a value from a key
    {
        CHECK_ARGC(2, "read");

        Key key = (Key) strtoul(command->args[1], NULL, 10);
        Value* value = database_actions_read_k(key);
        if (value == NULL)
        {
            RETURN_COMMAND_ERROR("unable to read <%u>", key);
        }
        snprintf(sendBuff, sizeof(sendBuff), "read : <%u,%s>", key, *value);
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
    }
    else if (strcmp(command->args[0], "delete") == 0) // delete a <k,v> from a key
    {
        CHECK_ARGC(2, "delete");
        ECHO();
    }
    else if (strcmp(command->args[0], "update") == 0) // update a <k,v> from a key
    {
        CHECK_ARGC(3, "update");
        ECHO();
    }
    else if (strcmp(command->args[0], "q") == 0) // shutdown the server
    {
        CHECK_ARGC(1, "q");
        ECHO();
    }
    else
    {
        log_info("Unknow command : %s\n", command->args[0]);
        ECHO();
    }

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