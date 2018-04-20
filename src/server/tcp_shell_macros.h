//
// Created by snipy on 20.04.18.
//

#ifndef KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H
#define KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H

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
                            debug("send following to the client :\n\t%s\n", sendBuff);\
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
                            debug("send following to the client :\n\t%s\n", sendBuff);\
                            write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
                            return STATUS_OK;\
                        }} while(0);

#define RETURN_COMMAND_ERROR(M, ...) do {\
                        snprintf(sendBuff, sizeof(sendBuff), COMMAND_ERROR " : " M, ##__VA_ARGS__);\
                        debug("send following to the client :\n\t%s\n", sendBuff);\
                        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
                        return STATUS_OK;\
                    }while(0);

#define RETURN_COMMAND_OK()\
    do{\
        snprintf(sendBuff, sizeof(sendBuff), COMMAND_OK);\
        debug("send following to the client :\n\t%s\n", sendBuff);\
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
        return STATUS_OK;\
    } while(0);

#define RETURN_COMMAND_EMPTY()\
    do{\
    snprintf(sendBuff, sizeof(sendBuff), COMMAND_EMPTY);\
    debug("send following to the client :\n\t%s\n", sendBuff);\
    write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
    return STATUS_OK;\
    } while(0);

#endif //KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H
