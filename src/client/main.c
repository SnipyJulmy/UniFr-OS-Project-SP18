//
// Created by snipy on 21.03.18.
// Update client tcp/ip by MP on 22.03.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include "../utils/debug.h"

#include "shell.h"

int main(int argc, char* argv[])
{
    int socket_fd = 0;
    char receive_buffer[1024];
    struct sockaddr_in server_addr;

    if (argc != 2)
    {
        log_err("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    memset(receive_buffer, '0', sizeof(receive_buffer));
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        log_err("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        log_err("Errno error : %s\n", strerror(errno));
        log_err("\n inet_pton error occurred\n");
        return 1;
    }

    if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        log_err("Connection failed : %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    shell_loop(socket_fd);
    return EXIT_SUCCESS;
}