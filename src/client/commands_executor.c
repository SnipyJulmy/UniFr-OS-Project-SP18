//
// Created by snipy on 22.05.18.
//

#include "commands_executor.h"
#include "../utils/debug.h"

#include <stdlib.h>

#define FILE_COMMAND_BUFFER_SIZE 1024
#define FILE_COMMAND_TOKEN_DELIMITER " \t\n\r\a"

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

FileCommand* file_command_create(char** args, int argc)
{
    FileCommand* command = malloc(1 * sizeof(FileCommand));
    check_mem_and_return(command, NULL);

    command->args = args;
    command->argc = argc;

    command->destroy = (void (*)(FileCommand*)) free;

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

int process_command_file(const char* filename)
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



        // free the command
        command->destroy(command);
    }

    fclose(file);
    return EXIT_SUCCESS;
}
