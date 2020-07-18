/*************************************************************************
	> File Name: client.c
	> Author: DRZ 
	> Mail: 1927157164@qq.com
	> Created Time: 2020年03月22日 星期日 20时22分00秒
 ************************************************************************/

#include "head.h"

/* functoin performance: 客户端创建一个socket连接
 * params: 传入服务器端的IP地址和端口号
 * return value: 返回创建的套接字文件描述符
 */

int socket_connect(char *host, int port) {
    int sockfd;
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)(&server), sizeof(server)) < 0) {
        perror("connect");
        return -1;;
    }

    return sockfd;
}
