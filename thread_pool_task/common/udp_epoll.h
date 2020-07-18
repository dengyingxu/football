/*************************************************************************
	> File Name: udp_epoll.h
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Jul 2020 07:54:01 PM CST
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
void add_event(int epollfd, int fd, int events);
void del_event(int epollfd, int fd, int events);
int udp_connect(int epollfd, struct sockaddr_in *serveraddr);
int udp_accept(int epollfd, int fd);
#endif
