/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Jul 2020 05:02:30 PM CST
 ************************************************************************/

#include "../common/color.h"
#include "../common/common.h"
#include "../common/udp_server.h"
#include "../common/head.h"
#include "../game.h"
char *conf = "./server.conf";

struct Map court;

int main(int argc, char **argv) {
    int opt, port = 0, listener;
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
    court.width = atoi(get_value(conf, "COLS"));
    court.heigth = atoi(get_value(conf, "LINES"));
    court.start.x = 1;
    court.start.y = 1;
    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

    DBG(GREEN"INFO"NONE": Server start on Port %d\n", port);

    pthread_create(&draw_t, NULL, draw, NULL);

    while(1) {
        //
    }
    return 0;
}

