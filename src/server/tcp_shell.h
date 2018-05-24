//
// Created by snipy on 05.04.18.
//

#ifndef KEY_VALUE_DATABASE_SHELL_H
#define KEY_VALUE_DATABASE_SHELL_H

#include "server_connection.h"

typedef struct Command Command;
struct Command
{
    // attributes
    char** args;
    int argc;
    char* original_line;

    // methods
    void (* destroy)(Command* self);
};

void tcp_shell_loop(ServerConnectionArgs* connectionArgs);
Command* tcp_shell_command_create(char** args, int argc);

#endif //KEY_VALUE_DATABASE_SHELL_H
