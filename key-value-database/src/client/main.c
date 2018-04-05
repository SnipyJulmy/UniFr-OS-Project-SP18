//
// Created by snipy on 21.03.18.
// Update client tcp/ip by MP on 22.03.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include "../debug.h"

#define BUFFER_LENGTH 1025

int main(int argc, char* argv[])
{
    int sockfd = 0;
    ssize_t n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("Errno error : %s\n", strerror(errno));
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        log_err("Connection failed : %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char sendBuff[BUFFER_LENGTH];
    snprintf(sendBuff, sizeof(sendBuff), "insert 1 \"AD\"\n");
    write(sockfd, sendBuff, strlen(sendBuff));

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
    {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF)
        {
            log_err("Fupts error : %s\n", strerror(errno));
        }
    }

    if (n < 0)
    {
        log_err("Read error : %s\n", strerror(errno));
    }

    return 0;
}