/*************************************************************************
	> File Name: udp_client.c
	> Author: 
	> Mail: 
	> Created Time: Thu 02 Jul 2020 02:32:27 PM CST
 ************************************************************************/
#include "head.h"

int socket_udp() {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    printf("Socket create.\n");
    return sockfd;
}
