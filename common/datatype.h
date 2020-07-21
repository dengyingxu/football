/*************************************************************************
	> File Name: datatype.h
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Jul 2020 03:06:20 PM CST
 ************************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H
#define MAXMSG 1024
struct LogRequest{
    char name[20];
    int team;//0 RED 1 BLUE
    char msg[512];

};

struct LogResponse{
    int type; //0 success 1 failed
    char msg[512];
};


struct Point{
    int x;
    int y;
};

struct User{
    int team;
    int fd; //用来存储用户的连接
    char name[20];
    int online;
    int flag;//未响应次数
    struct Point loc;

};

struct Map{
    int width;
    int heigth;
    struct Point start;
    int gate_width;
    int gate_heigth;
};

struct Ctrl{
    int carry;
    int kick;
    int dirx;
    int diry;
};


#define FT_TEST 0x01   // 服务端发送的心跳信息
#define FT_WALL 0x02   // 服务端以字符串方式广播游戏情况
#define FT_MSG 0x04    // 客户端发送的信息 服务端转发的信息
#define FT_ACK 0x08    // 客户端对心跳的ACK确认
#define FT_FIN 0x10    // 客户端、服务端下线前告知
#define FT_CTL 0x20    // 客户端发送的控制信息 json
#define FT_GAME 0x40   // 服务端向客户端广播的游戏实时画面 json
#define FT_SCORE 0x80  // 服务端对客户端广播的比分信息 json
#define FT_GAMEOVER 0x100  // 游戏结束


struct FootBallMsg {
    int type;
    int size;
    int team;
    char name[20];
    struct Ctrl ctl;
    char msg[MAXMSG];
};
#endif
