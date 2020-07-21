/*************************************************************************
	> File Name: game.h
	> Author: 
	> Mail: 
	> Created Time: Thu 02 Jul 2020 04:18:54 PM CST
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include <curses.h>

#define MAX 50
struct Map court;
WINDOW *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int height, int startx, int starty);
void destory_win(WINDOW *win);
void gotoxy(int x, int y);
void gotoxy_putc(int x, int y, int c);
void gotoxy_puts(int x, int y, char* s);
void w_gotoxy_putc(WINDOW *win, int x, int y, int c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);
void initfootball();
void *draw(void *arg);

#endif
