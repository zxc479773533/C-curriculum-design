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
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include "linklist.h"

// the socket queue length
#define QUEUE 20

// the port that server use
#define PORT 8080

// the max message size receive from client
#define BUFF_SIZE 1024

// the key used to build conncetion
#define KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


int base64_encode(char *in_str, int in_len, char *out_str) {
/*
* Function Name: base64_encode
* Module: Server
* Parameter: char *in_str, int in_len, char *out_str
* Return: int
* Use: Base64 encode function, used to build connection
*/

    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    if (in_str == NULL || out_str == NULL)
        return -1;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length-1] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}

int readline(char *buff, int pos, char *line) {
/*
* Function Name: readline
* Module: Server
* Parameter: char *buf, int pos, char *line
* Return: int
* Use: read a line string form buffer
*/ 

    int len = strlen(buff);
    for (; pos < len; pos++) {
        if (buff[pos] =='\r' && buff[pos + 1] == '\n')
            return pos + 2;
        else
            *(line++) = buff[pos];
    }
    return -1;
}

int shakehands(int sock_client) {
/*
* Function Name: shakehands
* Module: Server
* Parameter: int sock_client
* Return: int
* Use: shakehands with client and build connection
*/ 

    // definations
    int pos = 0;
    char request[BUFF_SIZE];
    char line[256];
    //Sec-WebSocket-Accept
    char sec_accept[32];
    //sha1 data
    char sha1_data[SHA_DIGEST_LENGTH + 1];
    //reponse head buffer
    char response[BUFF_SIZE];

    // receive request
    if (read(sock_client, request, sizeof(request)) <= 0)
        return -1;
    printf("Receive HTTP request from cilent :\n\n");
    printf("%s\n", request);

    // prase request
    do {

        // traversal lines
        bzero(&line, sizeof(line));
        pos = readline(request, pos, line);

        if (strstr(line, "Sec-WebSocket-Key") != NULL) {

            strcat(line, KEY);
            SHA1((unsigned char*)&line + 19, strlen(line + 19), (unsigned char*)&sha1_data);
            base64_encode(sha1_data, strlen(sha1_data), sec_accept);

            // build HTTP response
            sprintf(response, \
                "HTTP/1.1 101 Switching Protocols\r\n" \
                "Upgrade: websocket\r\n" \
                "Connection: Upgrade\r\n" \
                "Sec-WebSocket-Accept: %s\r\n" \
                "\r\n", sec_accept);
            printf("Send HTTP response to client :\n\n");
            printf("%s", response);

            // send response
            if (write(sock_client, response, strlen(response)) < 0)
                return -1;
            break;
        }

    } while((request[pos] != '\r' || request[pos + 1] != '\n') && pos != -1);
    return 0;
}

int main(void) {

    // create socket
    int sock_server;
    sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_server < 0) {
        printf("Creat socket failed!\n");
        return 1;
    }

    // author information
    printf("----------------------------------------\n");
    printf("Part: Backstage server\n");
    printf("Author: zxc479773533@github.com\n");
    printf("Copyright (C) 2017 Yue Pan\n");
    printf("----------------------------------------\n");

    // create server sockaddr struct
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // bind
    if (bind(sock_server, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("Bind error!\n");
        return 1;
    }

    // listen
    if (listen(sock_server, QUEUE) == -1) {
        printf("Listen error!\n");
        return 1;
    }

    // no error
    printf("\n");
    printf("Server working......\n");

    // create client socket sockaddr
    int count = 0;
    char buff[BUFF_SIZE];
    struct sockaddr_in clieaddr;
    u_int len = sizeof(clieaddr);

    // build connection
    int connect = accept(sock_server, (struct sockaddr*)&clieaddr, &len);
    shakehands(connect);

    // main
    while (1) {
        
        int sock_client;
        sock_client = accept(sock_server, (struct sockaddr*)&clieaddr, &len);

        printf("data : %d\n", ++count);
        bzero(&buff, sizeof(buff));
        int cmd_len = recv(sock_client, buff, sizeof(buff), 0);
        printf("%s\n", buff);
    }

    return 0;
}
