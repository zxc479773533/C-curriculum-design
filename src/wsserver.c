/*
* Part: Backstage server
* Module: Server achieve
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#include "wsserver.h"

// Base64 encode function, used to build connection
int base64_encode(char *in_str, int in_len, char *out_str) {

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

// read a line string form buffer
int readline(char *buff, int pos, char *line) {

    int len = strlen(buff);
    for (; pos < len; pos++) {
        if (buff[pos] =='\r' && buff[pos + 1] == '\n')
            return pos + 2;
        else
            *(line++) = buff[pos];
    }
    return -1;
}

// shakehands with client and build connection
int shakehands(int sock_client) {

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
    while ((request[pos] != '\r' || request[pos + 1] != '\n') && pos != -1) {

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
            write(sock_client, response, strlen(response));
            break;
        }

    }
    return 0;
}

// reverse string
void strreverse(char *str, int len) {
    int i;
    char tmp;
    for (i = 0; i< len/2; i++) {
        tmp = *(str + i);
        *(str + i) = *(str + len - i - 1);
        *(str + len - i - 1) = tmp;
    }
}

// receive message from client and parse it
int receive_and_parse(int fd, websocket_head *pWS) {

    char tmp;

    // get fin and opencode
    if (read(fd, &tmp, 1) <= 0)
        return -1;
    pWS->fin = ((tmp & 0x80) == 0x80);
    pWS->opencode = (tmp & 0x0F);

    // get mask and plyload length
    if (read(fd, &tmp, 1) <= 0)
        return -1;
    pWS->mask = ((tmp & 0x80) == 0X80);
    pWS->payload_length = (tmp & 0x7F);

    // the next two bytes is true payload length
    if (pWS->payload_length == 126) {
        char extern_len[2];
        if (read(fd, extern_len, 2) <= 0)
            return -1;
        pWS->payload_length = (extern_len[0] & 0xff) << 8 | (extern_len[1] & 0xff);
    }

    // the next eight bytes is the true payload length
    else if (pWS->payload_length == 127) {
        char extern_len[8];
        if (read(fd, &extern_len, 8) <= 0)
            return -1;
        strreverse(extern_len, 8);
        memcpy(&(pWS->payload_length), extern_len, 8);
    }

    // get masking key
    if (read(fd, pWS->masking_key, 4) <= 0)
        return -1;
    
    return 0;
}

// decode message
void DecodeMessage(char *data, int len, u_char *mask) {
    int i;
    for (i = 0; i < len; i++)
        *(data + i) ^=*(mask + (i % 4));
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

        websocket_head head;
        char payload[BUFF_SIZE];
        int size = 0;

        int Ecode = receive_and_parse(connect, &head);
        if (Ecode < 0)
            break;

        while (size < head.payload_length) {
            int readSize = read(connect, payload, 1024);
            if (readSize <= 0)
                break;
            size += readSize;
            DecodeMessage(payload, size, head.masking_key);
            printf("Receive message from client:\n\n");
            printf("%s\n", payload);
            write(connect, payload, readSize);
        }

    }

    return 0;
}
