/*************************************************************************
	> File Name: server.c
	> Author: DRZ 
	> Mail: 1927157164@qq.com
	> Created Time: 2020年03月22日 星期日 20时04分49秒
 ************************************************************************/

#include "head.h"

/* function performance: 服务端创建一个处于监听状态的套接字
 * params: 传入一个端口号
 * retval: 返回处于监听状态的套接字文件描述符
 */
int socket_create_udp(int port) {
    int server_listen;

    if ((server_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    make_nonblock(server_listen);
    if ((bind(server_listen, (struct sockaddr *)(&server), sizeof(server))) < 0) { 
        perror("bind");
        return -1;
    }
    
    return server_listen;
}
