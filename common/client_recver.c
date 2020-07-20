/*************************************************************************
	> File Name: client_recver.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2020 08:17:07 PM CST
 ************************************************************************/

#include "head.h"
#include "common.h"

extern int sockfd;

void *client_recv(void *arg) {
    while (1) {
        struct FootBallMsg msg;
        bzero(&msg, sizeof(msg));
        recv(sockfd, (void *)&msg,sizeof(msg), 0);
        if (msg.type & FT_TEST) {
            DBG(RED"HeartBeat from Server ♥"NONE"\n");
            msg.type = FT_ACK;//收到的报文结构重用了，修改信息回过去
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & (FT_MSG | FT_WALL)) {
            DBG(GREEN"Server Msg :"NONE"%s\n", msg.msg);
        } else if (msg.type & FT_FIN) {
            DBG(GREEN"Server is going to stop.\n"NONE);
            endwin();
            exit(0); //正常退出
        } else {
            DBG(GREEN"Msg Unsupport\n"NONE);
        }
    }
}


