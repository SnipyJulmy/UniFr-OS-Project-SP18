//
// Created by snipy on 27.03.18.
//

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "server_connection.h"
#include "../debug.h"

#define BUFFER_LENGTH 1025

static void free_args(ServerConnectionArgs* self)
{
    free(self->pthread);
    free(self);
}

ServerConnectionArgs* new_args(int connfd, pthread_t* pthread)
{
    ServerConnectionArgs* args = malloc(1 * sizeof(ServerConnectionArgs));
    args->free = free_args;
    args->connfd = connfd;
    args->pthread = pthread;
    return args;
}

static void parseArgs(char* string)
{

}

/**
 *
 * @param serverConnectionArgs
 * @return
 *
 * @warning the serverConnectionArgs parameter is free by the thread itself
 * @warning the pthread* inside the structure is freeing by the thread
 */
void* server_connection_handle(void* serverConnectionArgs)
{
    ServerConnectionArgs* args = serverConnectionArgs;
    pthread_t self = pthread_self();
    char sendBuff[BUFFER_LENGTH];
    char recvBuff[BUFFER_LENGTH];

    log_info("Thread %lu handle connection %d\n", self, args->connfd);

    ssize_t n;
    while ((n = read(args->connfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
    {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF)
        {
            log_err("Fupts error : %s\n",strerror(errno));
        }
    }

    time_t ticks = time(NULL);
    snprintf(sendBuff, sizeof(sendBuff), "Server provide you the local time: %.24s\r\n", ctime(&ticks));
    write(args->connfd, sendBuff, strlen(sendBuff));

    // close connection
    log_info("thread %lu close connection\n",self);
    close(args->connfd);

    // free the args
    log_info("thread %lu free arguments and self\n",self);
    args->free(args); // also free self !
    pthread_exit(NULL);
}
