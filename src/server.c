/*
* Part: Backstage server
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int main(void) {

    // create socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // create sockaddr struct
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(1234);
    bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    return 0;
}
