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
};

struct TransMsg{
    int dirx;//方向
    int diry;//方向
    struct Ctrl ctrl;
};

#define FT_TEST 0x01
#define FT_WALL 0x02
#define FT_MSG 0x04
#define FT_ACK 0x08


struct FootBallMsg {
    int type;
    int size;
    char msg[MAXMSG];
};
#endif
