//
// Created by snipy on 22.05.18.
//

#include "commands_executor.h"
#include "../utils/debug.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define MAKE_AND_SEND do {\
        mkSendBuffer(command);\
        log_info("send following command to the server :\n\t%s\n", sendBuff);\
        write(socket_fd, sendBuff, strlen(sendBuff));\
        } while(0);

#define CHECK_ARGC_AND_SEND_COMMAND(NB, COMMAND_NAME) do {\
        if(command->argc != (NB)) \
        {\
            log_warn("Invalid number of argument for : " COMMAND_NAME);\
            return STATUS_FAILURE;\
        }}while(0);\
        MAKE_AND_SEND;

#define CHECK_ARGC_AND_SEND_COMMAND2(A, B, COMMAND_NAME) do {\
        if(command->argc != (A) && command->argc != (B)) \
        {\
            log_warn("Invalid number of argument for : " COMMAND_NAME);\
            return STATUS_FAILURE;\
        }}while(0);\
        MAKE_AND_SEND;

#define ELAPSED_TIME(STATEMENT) do{\
            gettimeofday(&start, NULL); \
            STATEMENT \
            gettimeofday(&end, NULL); \
            sprintf(log_buffer, "[start]%li\n" \
                                "%s\n" \
                                "[end]%li\n\n", start.tv_usec, res, end.tv_usec); \
            fputs(log_buffer, log); \
            log_info("write into the log file"); \
            } while(0);

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

int process_command_file(const char* filename, const char* log_filename, int socket_fd)
{
    FILE* file = fopen(filename, "r");
    check_mem(file, {
        log_err("can't open file %s", filename);
        return EXIT_FAILURE;
    });

    FILE* log = fopen(log_filename, "a+");
    check_mem(log, {
        fclose(file);
        log_err("can't create or open file %s", log_filename);
        return EXIT_FAILURE;
    });

    char line[512];

    char log_buffer[512];
    struct timeval start;
    struct timeval end;

    log_info("for each line");
    while (fgets(line, sizeof(line), file))
    {
        log_info("line value : %s",line);
        // tokenize the line
        FileCommand* command = file_command_tokenize_line(line);

        if (strcmp(command->args[0], "add") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND2(2, 3, "add");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "ls") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(1, "ls");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "read_v") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "read_v");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "read_k") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "read_k");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "rm_k") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "rm_k");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "rm_v") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "rm_v");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "update_kv") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(3, "update_kv");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "delete_value") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "delete_value");
                    char* res = fetchTcpLine(socket_fd);
            );
        }
        else if (strcmp(command->args[0], "read_key") == 0)
        {
            ELAPSED_TIME(
                    CHECK_ARGC_AND_SEND_COMMAND(2, "read_key");
                    char* res = fetchTcpLine(socket_fd);
            );
        }

        // free the command
        command->destroy(command);
    }

    fclose(file);
    fclose(log);
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
