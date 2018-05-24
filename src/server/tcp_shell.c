//
// Created by snipy on 05.04.18.
//

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tcp_shell.h"
#include "../utils/debug.h"
#include "../utils/command_interface.h"
#include "../data_structures/dequeue/dequeue.h"
#include "database/database_actions.h"
#include "tcp_shell_macros.h"

#define SHELL_BUFFER_SIZE 4096
#define SHELL_TOKEN_DELIMITER " \t\n\r\a"

#define STATUS_FAILURE 2
#define STATUS_OK 1
#define STATUS_EXIT 0


// server buffer to send back information to the client

static char* tcp_shell_read_line(ServerConnectionArgs* connectionArgs);
static Command* tcp_shell_tokenize_line(char* line);
static int tcp_shell_execute(Command* command, ServerConnectionArgs* connectionArgs, char* sendBuff);
static void tcp_shell_command_destroy(Command* self);

void tcp_shell_loop(ServerConnectionArgs* connectionArgs)
{
    int status;
    char sendBuff[SHELL_BUFFER_SIZE];

    log_info("conn info : %d %p\n", connectionArgs->connfd, connectionArgs->pthread);

    do
    {
        char* line = tcp_shell_read_line(connectionArgs);
        log_info("execute command : %s", line);
        if (line == NULL)
        {
            log_err("line is null when reading from the tcp socket");
            goto end;
        }
        Command* command = tcp_shell_tokenize_line(line);
        status = tcp_shell_execute(command, connectionArgs, sendBuff);
        command->destroy(command);
    } while (status != STATUS_EXIT);
    end:;
}

Command* tcp_shell_command_create(char** args, int argc)
{
    Command* command = malloc(1 * sizeof(Command));
    check_mem_and_exit(command);
    command->args = args;
    command->argc = argc;
    command->destroy = tcp_shell_command_destroy;
    return command;
}

void tcp_shell_command_destroy(Command* self)
{
    free(self->args);
    free(self);
}

int tcp_shell_execute(Command* command, ServerConnectionArgs* connectionArgs, char* sendBuff)
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
                                      sizeof(char) * SHELL_BUFFER_SIZE,
                                      "<%u,%s>\n",
                                      elt->key, elt->value);
            }
        else
            RETURN_COMMAND_EMPTY();
        debug("send following to the client :\n\t%s\n", sendBuff);
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
            Value* value = malloc(1 * sizeof(char*));
            check_mem_and_exit(value);
            value[0] = malloc(strlen(command->args[1]) * sizeof(char));
            check_mem_and_exit(value[0]);
            strcpy(value[0], command->args[1]);
            key = database_actions_insert_v(value);
            if (key == 0)
            {
                RETURN_COMMAND_ERROR("unable to insert value <%s>", *value);
            }
            snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE, "key2 : %u", key);
        }
        else // command->argc == 3
        {
            Value* value = malloc(1 * sizeof(char*));
            check_mem_and_exit(value);
            value[0] = malloc(strlen(command->args[2]) * sizeof(char));
            check_mem_and_exit(value[0]);

            strcpy(value[0], command->args[2]);
            key = (Key) strtoul(command->args[1], NULL, 10);
            bool status = database_actions_insert_kv(key, value);
            if (!status)
                RETURN_COMMAND_ERROR("unable to add <%u,%s>", key, *value);
            snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE, "key3 : %u", key);
        }
        debug("send following to the client :\n\t%s\n", sendBuff);
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
        return STATUS_OK;
    }
    else if (strcmp(command->args[0], "read_k") == 0) // read a key from a value
    {
        CHECK_ARGC(2, "read_k");
        Value value = command->args[1];
        Key key = database_actions_read_k_from_v(&value);
        if (key == NULL)
            RETURN_COMMAND_ERROR("unable to find a key for the value %s", value);
        snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE, "read : <%u,%s>", key, value);
        debug("send following to the client :\n\t%s\n", sendBuff);
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
    }
    else if (strcmp(command->args[0], "read_v") == 0) // read a value from a key
    {
        CHECK_ARGC(2, "read_v");

        Key key = (Key) strtoul(command->args[1], NULL, 10);
        Value* value = database_actions_read_v_from_key(key);
        if (value == NULL)
        {
            RETURN_COMMAND_ERROR("unable to read <%u>", key);
        }
        snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE, "read : <%u,%s>", key, *value);
        debug("send following to the client :\n\t%s\n", sendBuff);
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));
    }
    else if (strcmp(command->args[0], "rm_k") == 0) // delete a <k,v> from a key
    {
        CHECK_ARGC(2, "rm_k");
        Key key = (Key) strtoul(command->args[1], NULL, 10);
        Value value = command->argc == 2 ?
                      "_" :
                      command->args[2];
        bool status = command->argc == 2 ?
                      database_actions_remove_from_k(key) :
                      database_actions_remove_from_kv(key, &value);
        if (!status)
            RETURN_COMMAND_ERROR("unable to delete <%u,%s>", key, value);
        RETURN_COMMAND_OK();
    }
    else if (strcmp(command->args[0], "rm_v") == 0) // delete a <k,v> given a value
    {
        CHECK_ARGC(2, "rm_v");
        Value value = command->args[1];
        bool status = database_actions_remove_from_v(&value);
        if (!status)
            RETURN_COMMAND_ERROR("unable to delete a entry given the value %s", value);
        RETURN_COMMAND_OK();
    }
    else if (strcmp(command->args[0], "update_kv") == 0) // update a <k,v> from a key
    {
        CHECK_ARGC(3, "update_kv");
        Key key = (Key) strtoul(command->args[1], NULL, 10);
        Value* value = malloc(1 * sizeof(value));
        value[0] = malloc(strlen(command->args[2]) * sizeof(char));
        strcpy(*value, command->args[2]);
        debug("value on update is %s", *value);
        bool status = database_actions_update_kv(key, value);
        if (!status)
            RETURN_COMMAND_ERROR("unable to update <%u,%s>", key, *value);
        RETURN_COMMAND_OK();
    }
    else if (strcmp(command->args[0], "q") == 0) // shutdown the server
    {
        CHECK_ARGC(1, "q");
        RETURN_COMMAND_EXIT();
    }
    else
    {
        log_info("Unknow command : %s\n", command->args[0]);
        ECHO();
    }

    return STATUS_OK;
}

Command* tcp_shell_tokenize_line(char* line)
{
    int buffer_size = SHELL_BUFFER_SIZE;
    int position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;
    char* savedptr = line;

    check_mem_and_exit(tokens);
    token = strtok_r(line, SHELL_TOKEN_DELIMITER, &savedptr);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            check_mem_and_exit(tokens);
        }

        token = strtok_r(NULL, SHELL_TOKEN_DELIMITER, &savedptr);
    }
    tokens[position] = NULL;
    Command* res = tcp_shell_command_create(tokens, position);
    return res;
}

// read a line from the command line interface
char* tcp_shell_read_line(ServerConnectionArgs* connectionArgs)
{
    ssize_t n;
    int buffer_size = SHELL_BUFFER_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));

    while ((n = read(connectionArgs->connfd, buffer, sizeof(char) * (buffer_size - 1))) <= 0);
    if (n <= 0)
        return NULL;
    buffer[n] = '\0';
    return buffer;
}