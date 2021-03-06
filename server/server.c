/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Jul 2020 05:02:30 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/udp_server.h"
#include "../common/udp_epoll.h"
#include "../common/game.h"
#include "../common/thread_pool.h"
#include "../common/sub_reactor.h"
#include "../common/heart_beat.h"
#include "../common/server_exit.h"
char *conf = "./server.conf";

struct User *rteam;
struct User *bteam;
int repollfd, bepollfd;
int data_port;
int port = 0;
//struct Map court;

int main(int argc, char **argv) {
    int opt, listener;
    int epoll_fd;
    pthread_t draw_t;//创建多线程，在服务端绘制画面
    pthread_t red_t, blue_t, heart_t;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);//optarg是getopt的全局变量
                break;
            default:
                fprintf(stderr, "Usage: %s [-p sort]\n", argv[0]);
                exit(1);
        }
    }
    argc -= (optind - 1);//个数是减少
    argv += (optind - 1);//指针是后移

    if (argc > 1) {
        fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
        exit(1);
    }
    
    if (!port) port = atoi(get_value(conf, "PORT"));
    //data_port = atoi(get_value(conf, "DATAPORT"));
    court.width = atoi(get_value(conf, "COLS"));
    court.heigth = atoi(get_value(conf, "LINES"));
    court.start.x = 1;
    court.start.y = 1;

    rteam = (struct User *)calloc(MAX, sizeof(struct User));
    bteam = (struct User *)calloc(MAX, sizeof(struct User));

    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

    DBG(GREEN"INFO"NONE": Server start on Port %d\n", port);

#ifndef _D
    pthread_create(&draw_t, NULL, draw, NULL);
#endif

    epoll_fd = epoll_create(MAX * 2);
    repollfd = epoll_create(MAX);
    bepollfd = epoll_create(MAX);

    if (epoll_fd < 0 || repollfd < 0 || bepollfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    struct task_queue redQueue;
    struct task_queue blueQueue;

    task_queue_init(&redQueue, MAX, repollfd);
    task_queue_init(&blueQueue, MAX, bepollfd);

    //创建从反应堆线程
    pthread_create(&red_t, NULL, sub_reactor, (void *)&redQueue);
    //创建从反应堆线程
    pthread_create(&blue_t, NULL, sub_reactor, (void *)&blueQueue);
    //创建心跳线程
    pthread_create(&heart_t, NULL, heart_beat, NULL);
   
    signal(SIGINT, server_exit);

    struct epoll_event ev, events[MAX * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev);
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    Show_Message(, , "Waitong for Login.", 1);
    while(1) {
        DBG(YELLOW"Main Thread"NONE" : before epoll_wait\n");
        int nfds = epoll_wait(epoll_fd, events, MAX * 2, -1);
        DBG(YELLOW"Main Thread"NONE" : Atfer epoll_wait\n");

        for (int  i = 0; i < nfds; i++) {
            struct User user;
            char buff[512] = {0};
            DBG(YELLOW"EPOLL"NONE" : Doing with %dth fd\n", i);
            if (events[i].data.fd  == listener) {
                //accept();
                int new_fd = udp_accept(epoll_fd, listener, &user);
                if (new_fd > 0) {
                    sprintf(buff, "%s Login. the Game.", user.name);
                    DBG(YELLOW"Main Thread"NONE" : Add %s to %s sub_reactor.\n", user.name, (user.team ? "BLUE" : "RED"));
                    add_to_sub_reactor(&user);
                    Show_Message(, , buff, 1);
                }
            } else {
                recv(events[i].data.fd, buff, sizeof(buff), 0);
                printf(PINK"RECV"NONE" : %s\n", buff);
            }
            //char info[1024] = {0};
            //w_gotoxy_puts(Message, 1, 2, info);
        }
    }
    return 0;
}

