/*************************************************************************
	> File Name: sub_reactor.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2020 10:53:49 AM CST
 ************************************************************************/

#include "head.h"
#include "thread_pool.h"
#include "sub_reactor.h"
#define MAX 50

void *sub_reactor(void *arg) {
    struct task_queue *taskQueue = (struct task_queue *)arg;
    pthread_t *tid = (pthread_t *)calloc(MAXTHREAD, sizeof(pthread_t));
    for (int i = 0; i < MAXTHREAD; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)taskQueue);
    }
    //处理反应堆事件
    struct epoll_event ev, events[MAX];
    int nfds;
    while (1) {
        DBG(RED"Sub Reactor: "NONE" Epoll waiting...\n");
        nfds = epoll_wait(taskQueue->epollfd, events, MAX, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(1);//这个程序中的所有线程都会结束
        }
        for (int i = 0; i < nfds; i++) {
           /* DBG(L_YELLOW"events[n].data.ptr->name = %s"NONE"\n", (struct User *)events[i].data.ptr->name);
            if (events[i].events & EPOLLIN) {
                task_queue_push(taskQueue, (struct User *)events[i].data.ptr);
            }*/
            struct User *user = (struct User *)events[i].data.ptr;
            DBG(L_YELLOW"events[n].data.ptr->name = %s"NONE"\n", user->name);
            if (events[i].events & EPOLLIN) {
                task_queue_push(taskQueue, user);
            }
        }
    }
    return NULL;
}
