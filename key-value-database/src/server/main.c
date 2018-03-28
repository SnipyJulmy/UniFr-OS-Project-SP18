//
// Created by snipy on 21.03.18.
// Update server tcp/ip by MP on 22.03.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "server_connection.h"
#include "../debug.h"

int main(void)
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1)
    {
        // accept a connection
        printf("Waiting on a connection on port %d\n",ntohs(serv_addr.sin_port));
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
        printf("Connection accepted !\n");
        // create a thread that will take care of the connection
        pthread_t* pthread = malloc(sizeof(pthread_t));
        ServerConnectionArgs* args = new_args(connfd, pthread);
        if ((pthread_create(pthread, NULL, server_connection_handle, args)) != 0)
        {
            log_err("Can't create thread to handle connection %d\n", connfd);
        }

        sleep(1);
        // TODO Idea :
        /*
         *  Add a stop command with password etc...
         *  to stop the server manually if needed
         */
    }
    #pragma clang diagnostic pop
}
