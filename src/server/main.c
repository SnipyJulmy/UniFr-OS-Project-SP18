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
#include "../utils/debug.h"
#include "database/database_actions.h"

int iSetOption =1; // added options for socket connection

int main(void)
{
    // init
    database_actions_init();

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //This line allow to reconnect without delai and probably sort the connection problem
    // while starting and closing server multiple times and bad port issue (MP)
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

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
        log_info("Waiting on a connection on port %d\n", ntohs(serv_addr.sin_port));
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
        log_info("Connection accepted !\n");

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