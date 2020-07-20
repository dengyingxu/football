/*************************************************************************
	> File Name: server_exit.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2020 09:09:40 PM CST
 ************************************************************************/

#include "head.h"
#define MAX 50
extern struct User *rteam, *bteam;

void server_exit(int signum) {
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    for (int i = 0; i < MAX; i++) {
        if (rteam[i].online) send(rteam[i].fd, (void *)&msg, sizeof(msg), 0);
        if (bteam[i].online) send(bteam[i].fd, (void *)&msg, sizeof(msg), 0);
    }
    endwin();
    DBG(RED"Server stopped!\n"NONE);
    exit(0);
}
