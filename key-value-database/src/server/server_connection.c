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
    char sendBuff[1025];

    printf("Thread %d handle connection %d", self, args->connfd);

    time_t ticks = time(NULL);
    snprintf(sendBuff, sizeof(sendBuff), "Server provide you the local time: %.24s\r\n", ctime(&ticks));
    write(args->connfd, sendBuff, strlen(sendBuff));

    // close connection
    close(args->connfd);

    // free the args
    args->free(args);

    pthread_exit(NULL);
}
