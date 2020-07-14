/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Jul 2020 05:02:30 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/udp_server.h"
#include "../common/udp_epoll.h"
#include "../game.h"
char *conf = "./server.conf";

struct User *rteam;
struct User *bteam;
int data_port;
int port = 0;
//struct Map court;

int main(int argc, char **argv) {
    int opt, listener;
    int epoll_fd;
    pthread_t draw_t;//创建多线程，在服务端绘制画面
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
    data_port = atoi(get_value(conf, "DATAPORT"));
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

//    pthread_create(&draw_t, NULL, draw, NULL);

    epoll_fd = epoll_create(MAX * 2);

    if (epoll_fd < 0) {
        perror("epoll_create");
        exit(1);
    }
    
    struct epoll_event ev, events[MAX * 2];

    ev.events = EPOLLIN;
    ev.data.fd = listener;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev);
    struct LogData lg;
    struct sockaddr_in client;

    socklen_t len = sizeof(client);

    while(1) {
        //w_gotoxy_puts(Message, 1, 1, "Wait for Login\n");
        //wrefresh(Message);
        int nfds = epoll_wait(epoll_fd, events, MAX * 2, -1);

        for (int  i = 0; i < nfds; i++) {
            if (events[i].data.fd  == listener) {
                //accept();
                udp_accept(epoll_fd, listener);
            }
            //char info[1024] = {0};
            //recvfrom(events[i].data.fd, (void *)info, sizeof(info), 0, (struct sockaddr *)&client, &len);
            //sprintf(info, "Login: %s : %d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            //w_gotoxy_puts(Message, 1, 2, info);
        }
    }
    return 0;
}

