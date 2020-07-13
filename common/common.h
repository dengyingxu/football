/*************************************************************************
	> File Name: common.h
	> Author: DRZ 
	> Mail: 1927157164@qq.com
	> Created Time: 2020年03月30日 星期一 14时25分16秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

char *get_value(char *, char *);

void make_nonblock_ioctl(int);
void make_block_ioctl(int);

void make_nonblock(int);
void make_block(int);

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif


#endif
