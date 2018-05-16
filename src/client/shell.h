//
// Created by snipy on 05.04.18.
//

#ifndef KEY_VALUE_DATABASE_SHELL_H
#define KEY_VALUE_DATABASE_SHELL_H

typedef struct Command Command;
struct Command
{
    // attributes
    char** args;
    int argc;

    // methods
    void (* destroy)(Command* self);
};

Command* shell_command_create(char** args, int argc);
void shell_loop(int socket_fd);

#endif //KEY_VALUE_DATABASE_SHELL_H
