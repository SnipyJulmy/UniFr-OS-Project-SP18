//
// Created by snipy on 27.03.18.
//

#ifndef KEY_VALUE_DATABASE_SERVER_CONNECTION_H
#define KEY_VALUE_DATABASE_SERVER_CONNECTION_H

#define SERVER_CONNECTION_BUFFER_SIZE 100;

typedef struct ServerConnectionArgs ServerConnectionArgs;
struct ServerConnectionArgs
{
    int connfd;
    pthread_t* pthread;
    void (* free)(ServerConnectionArgs*);
};

// return a pointer to a ServerConnectionArgs
// have to invoke ServerConnectionArgs->free() in order to free the resources
ServerConnectionArgs* new_args(int connfd, pthread_t* pthread);
void* server_connection_handle(void* serverConnectionArgs);

#endif //KEY_VALUE_DATABASE_SERVER_CONNECTION_H
