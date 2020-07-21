/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu 16 Jul 2020 05:21:41 PM CST
 ************************************************************************/

#include "thread_pool.h"
#include "udp_epoll.h"
#include "game.h"
extern int repollfd, bepollfd;

void do_echo(struct User *user) {
    struct FootBallMsg msg;
    char tmp[512] = {0};
    int size = recv(user->fd, (void *)&msg, sizeof(msg), 0);
    user->flag = 10;
    //user->online = 1;
    if (msg.type & FT_ACK) {
        if (user->team) 
            DBG(L_BLUE" %s "NONE"❤ \n", user->name);
        else 
            DBG(L_RED" %s "NONE"❤ \n", user->name);
    } else if (msg.type & (FT_WALL | FT_MSG)) {
        if (user->team) 
            DBG(L_BLUE" %s : %s\n"NONE, user->name, msg.msg);
        else 
            DBG(L_RED" %s : %s\n"NONE, user->name, msg.msg);
        strcpy(msg.name, user->name);
        msg.team = user->team;
        Show_Message(, user, msg.msg, );
        send(user->fd, (void *)&msg, sizeof(msg), 0);
    } else if (msg.type & FT_FIN) {
        DBG(RED"%s logout.\n", user->name);
        sprintf(tmp, "%s logout.", user->name);
        Show_Message(, NULL, tmp, 1);
        user->online = 0;
        int epollfd_tmp = (user->team ? bepollfd : repollfd);
        del_event(epollfd_tmp, user->fd);
    }
}



void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd) {
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *));
    taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
};

void task_queue_push(struct task_queue *taskQueue, struct User *user) {
    pthread_mutex_lock(&taskQueue->mutex);
    taskQueue->team[taskQueue->tail] = user;
    DBG(L_GREEN"Thread Pool:"NONE"Task Push %s\n", user->name);
    if (++taskQueue->tail == taskQueue->sum) {
        DBG(L_GREEN"Thread poll : "NONE" Task Queue End.\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    while (taskQueue->tail == taskQueue->head) {
        DBG(L_GREEN"THread poop : "NONE"Task Queue Empty, Waiting For Task.\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    struct User *user = taskQueue->team[taskQueue->head];
    DBG(L_GREEN"Thread pool : "NONE"Task Pop %s.\n", user->name);
    if (++taskQueue->head == taskQueue->sum) {
        DBG(L_GREEN"Thread poll : "NONE" Task Queue End.\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run(void *arg) {
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_echo(user);
    }
}
