/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Fri 03 Jul 2020 08:01:50 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/udp_client.h"
int main() {
struct sockaddr_in dest_addr ;
dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(8888);
dest_addr.sin_addr.s_addr = inet_addr("120.26.176.60");
    int sockfd = socket_udp();
    while(1) {
        sleep(2);
        char buf[1024];

        //size_t len = sizeof(buf);
        int l = sendto(sockfd, (void *)buf, sizeof(buf), 0,
              (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (l > 0) printf("good\n");
        else printf("bed\n");
    } 
}
