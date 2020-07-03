/*************************************************************************
	> File Name: common.c
	> Author: DRZ 
	> Mail: 1927157164@qq.com
	> Created Time: 2020年03月30日 星期一 14时23分12秒
 ************************************************************************/

#include "head.h"

//采用ioctl()和fcntl()函数实现阻塞和非阻塞IO
//文件一般默认的属性是阻塞状态


/* function performance: 从配置文件中读取要的数据
 * params: path配置文件的路径和名称, key要的数据类型
 * return value: 返回需要的数据
 */
char *get_value(char *path, char *key) {
    FILE *fp = NULL;
    if (path == NULL || key == NULL) {
        fprintf(stderr, "error in  argument\n");
        return NULL;
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopeen");
        return NULL;
    }

    ssize_t nrd;
    char *line = NULL, *sub = NULL;
    size_t linecap;
    extern char conf_ans[50]; //ans需要返回，故采用全局变量

    //逐行读入文件内容,getline()函数会读取换行符
    while ((nrd = getline(&line, &linecap, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) { //该行没有
            continue;
        } else {
            if (line[strlen(key)] == '=') {
                strncpy(conf_ans, sub + strlen(key) + 1, nrd - strlen(key) - 2);
                *(conf_ans + nrd - strlen(key) - 2) = '\0';
                break;
            }
        }
    }
    free(line);
    fclose(fp);
    if (sub == NULL) {
        return NULL;
    }
    return conf_ans;
}


void make_nonblock_ioctl(int fd) {
    unsigned long ul = 1;
    ioctl(fd, FIONBIO, &ul);
}

void make_block_ioctl(int fd) {
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}

void make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return ;
    }
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

void make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return ;
    }
    flag &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}


