//
// Created by snipy on 22.05.18.
//

#include "commands_executor.h"
#include "../utils/debug.h"

#include <stdlib.h>
#include <unistd.h>

#define MAKE_AND_SEND do {\
        mkSendBuffer(command);\
        log_info("send following command to the server :\n\t%s\n", sendBuff);\
        write(socket_fd, sendBuff, strlen(sendBuff));\
        } while(0);

#define CHECK_ARGC_AND_SEND_COMMAND(NB, COMMAND_NAME) do {\
        if(command->argc != (1+(NB))) \
        {\
            log_warn("Invalid number of argument for : " COMMAND_NAME);\
            return STATUS_FAILURE;\
        }}while(0);\
        MAKE_AND_SEND;

#define CHECK_ARGC_AND_SEND_COMMAND2(A, B, COMMAND_NAME) do {\
        if(command->argc != ((A)+1) && command->argc != ((B)+1)) \
        {\
            log_warn("Invalid number of argument for : " COMMAND_NAME);\
            return STATUS_FAILURE;\
        }}while(0);\
        MAKE_AND_SEND;

#define FILE_COMMAND_BUFFER_SIZE 1024
#define FILE_COMMAND_TOKEN_DELIMITER " \t\n\r\a"

#define STATUS_FAILURE 2
#define STATUS_OK 1
#define STATUS_EXIT 0

typedef struct FileCommand FileCommand;

struct FileCommand
{
    // Attributes
    char** args;
    int argc;

    // Methods
    void (* destroy)(FileCommand* self);
};

static FileCommand* file_command_tokenize_line(char* line);
static FileCommand* file_command_create(char** args, int argc);
static void file_command_destroy(FileCommand* self);

// client buffer to send command to the server
char sendBuff[1000];
static void mkSendBuffer(FileCommand* command);
static char* fetchTcpLine(int socket_fd);

FileCommand* file_command_create(char** args, int argc)
{
    FileCommand* command = malloc(1 * sizeof(FileCommand));
    check_mem_and_return(command, NULL);

    command->args = args;
    command->argc = argc;

    command->destroy = file_command_destroy;

    return command;
}

FileCommand* file_command_tokenize_line(char* line)
{
    int buffer_size = FILE_COMMAND_BUFFER_SIZE;
    int position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;

    check_mem_and_exit(tokens);
    token = strtok(line, FILE_COMMAND_TOKEN_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            check_mem_and_exit(tokens);
        }

        token = strtok(NULL, FILE_COMMAND_TOKEN_DELIMITER);
    }
    tokens[position] = NULL;
    return file_command_create(tokens, position);
}

int process_command_file(const char* filename, int socket_fd)
{
    FILE* file = fopen(filename, "r");
    check_mem(file, {
        log_err("can't open file %s", filename);
        return EXIT_FAILURE;
    });

    char line[512];

    log_info("for each line");
    while (fgets(line, sizeof(line), file))
    {
        // tokenize the line
        FileCommand* command = file_command_tokenize_line(line);

        if (strcmp(command->args[0], "add") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND2(2, 3, "add");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }
        else if (strcmp(command->args[0], "read") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND(2, "read");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }
        else if (strcmp(command->args[0], "delete") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND(2, "delete");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }
        else if (strcmp(command->args[0], "update") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND(3, "update");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }
        else if (strcmp(command->args[0], "delete_value") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND(2, "delete_value");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }
        else if (strcmp(command->args[0], "read_key") == 0)
        {
            CHECK_ARGC_AND_SEND_COMMAND(2, "read_key");
            char* res = fetchTcpLine(socket_fd);
            printf("%s\n", res);
        }

        // free the command
        command->destroy(command);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

void file_command_destroy(FileCommand* self)
{
    free(self->args);
    free(self);
}

// side effect : modify the global variable sendBuff
void mkSendBuffer(FileCommand* command)
{
    // write the command name inside the buffer with a whitespace
    snprintf(sendBuff, sizeof(sendBuff), "%s", command->args[0]);
    size_t size_used = strlen(command->args[0]);

    for (int i = 1; i < command->argc - 1; i++)
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

char* fetchTcpLine(int socket_fd)
{
    ssize_t n;
    int buffer_size = FILE_COMMAND_BUFFER_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));

    n = read(socket_fd, buffer, sizeof(char) * (buffer_size - 1));
    if (n <= 0)
        return NULL;
    buffer[n] = 0;
    return buffer;
}
