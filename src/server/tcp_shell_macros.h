//
// Created by snipy on 20.04.18.
//

#ifndef KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H
#define KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H

// echo on command->args[0]
#define ECHO() \
    do {\
    snprintf(sendBuff,  sizeof(char) * SHELL_BUFFER_SIZE , "echo %s", command->args[0]); \
    write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
    return STATUS_OK; \
    } while(0);

// -- For argc control

#define CHECK_ARGC(NB, COMMAND_NAME)\
                        do {\
                        log_info("check argc for " COMMAND_NAME); \
                        if(command->argc != (NB)) \
                        { \
                            log_warn("Invalid number of argument for : " COMMAND_NAME \
                                     " expected %d get %d"\
                                     ,NB,command->argc);\
                            snprintf(sendBuff,  sizeof(char) * SHELL_BUFFER_SIZE , COMMAND_ERROR); \
                            debug("send following to the client :\n\t%s\n", sendBuff);\
                            write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
                            return STATUS_OK;\
                        }} while(0);

#define CHECK_ARGC_2(A, B, COMMAND_NAME)\
                        do {\
                        log_info("check argc for " COMMAND_NAME); \
                        if(command->argc != (A) && command->argc != (B)) \
                        { \
                            log_warn("Invalid number of argument for : " COMMAND_NAME \
                                     " expected %d or %d get %d " \
                                     ,A,B,command->argc);\
                            /* for(int i=0;i<command->argc;i++) printf("%s\n",command->args[i]); */\
                            snprintf(sendBuff,  sizeof(char) * SHELL_BUFFER_SIZE , COMMAND_ERROR); \
                            debug("send following to the client :\n\t%s\n", sendBuff);\
                            write(connectionArgs->connfd, sendBuff, strlen(sendBuff)); \
                            return STATUS_OK;\
                        }} while(0);

#define RETURN_COMMAND_ERROR(M, ...) do {\
                        snprintf(sendBuff,  sizeof(char) * SHELL_BUFFER_SIZE , COMMAND_ERROR " : " M, ##__VA_ARGS__);\
                        debug("send following to the client :\n\t%s\n", sendBuff);\
                        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
                        return STATUS_OK;\
                    }while(0);

#define RETURN_COMMAND_OK()\
    do{\
        snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE , COMMAND_OK);\
        debug("send following to the client :\n\t%s\n", sendBuff);\
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
        return STATUS_OK;\
    } while(0);

#define RETURN_COMMAND_EXIT()\
    do{\
        snprintf(sendBuff,  sizeof(char) * SHELL_BUFFER_SIZE , COMMAND_OK);\
        debug("send following to the client :\n\t%s\n", sendBuff);\
        write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
        return STATUS_EXIT;\
    } while(0);

#define RETURN_COMMAND_EMPTY()\
    do{\
    snprintf(sendBuff, sizeof(char) * SHELL_BUFFER_SIZE, COMMAND_EMPTY);\
    debug("send following to the client :\n\t%s\n", sendBuff);\
    write(connectionArgs->connfd, sendBuff, strlen(sendBuff));\
    return STATUS_OK;\
    } while(0);

#endif //KEY_VALUE_DATABASE_TCP_SHELL_MACROS_H
