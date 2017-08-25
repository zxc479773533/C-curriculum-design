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
    char line[LINE_SIZE];
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


// send message to client
int SendMessage(int fd, char *payload, int payload_length) {

    char *buffer;
    int head_length;

    if (payload_length < 126) {
        buffer = (char *)malloc(2 + payload_length);
        buffer[0] = 0x81;
        buffer[1] = payload_length;
        head_length = 2;
    }

    else if (payload_length < 0xffff) {
        buffer = (char *)malloc(4 + payload_length);
        buffer[0] = 0x81;
        buffer[1] = 126;
        buffer[2] = (payload_length >> 8 & 0xff);
        buffer[3] = (payload_length & 0xff);
        head_length = 4;
    }

    else {
        buffer = (char *)malloc(12 + payload_length);
        buffer[0] = 0x81;
        buffer[1] = 127;
        memcpy(buffer + 2, &payload_length, sizeof(unsigned long long));
        strreverse(buffer + 2, sizeof(unsigned long long));
        head_length = 12;
    }

    // send packet
    memcpy(buffer + head_length, payload, payload_length);
    if (write(fd, buffer, head_length + payload_length) <= 0)
        return -1;

    free(buffer);
    return 0;
}

// load data from file
void LoadData(Linklist *L) {
    
    // open data files
    FILE *fpl, *fps, *fpc;
    if ((fpl = fopen("/tmp/.c-curriculum-design/linedb", "rb")) == NULL)
        return ;
    if ((fps = fopen("/tmp/.c-curriculum-design/stationdb", "rb")) == NULL)
        return ;
    if ((fpc = fopen("/tmp/.c-curriculum-design/cardb", "rb")) == NULL)
        return ;

    // create three linklist to keep the data
    int size = 1;
    FirstNode Line_H, *tail_L = Line_H.next;
    SecondNode Station_H, *tail_S = Station_H.next;
    ThirdNode Car_H, *tail_C = Car_H.next;

    while (size == 1) {
        FirstNode *tmp_L = (FirstNode *)malloc(sizeof(FirstNode));
        size = fread(tmp_L, sizeof(FirstNode), 1, fpl);
        tail_L = tmp_L;
        tail_L = tail_L->next;
    }

    size = 1;
    while (size == 1) {
        SecondNode *tmp_S = (SecondNode *)malloc(sizeof(SecondNode));
        size = fread(tmp_S, sizeof(SecondNode), 1, fps);
        tail_S = tmp_S;
        tail_S = tail_S->next;
    }

    size = 1;
    while (size == 1) {
        ThirdNode *tmp_C = (ThirdNode *)malloc(sizeof(ThirdNode));
        size = fread(tmp_C, sizeof(ThirdNode), 1, fpc);
        tail_C = tmp_C;
        tail_C = tail_C->next;
    }

    // create main linklist
    ListInitial(L);
    tail_L = Line_H.next;
    while (tail_L != NULL) {
        ListInsert_F(L, tail_L->LineInfo);
        tail_L = tail_L->next;
    }
    tail_S = Station_H.next;
    while (tail_S != NULL) {
        ListInsert_S(L, tail_S->StationInfo);
        tail_S = tail_S->next;
    }
    tail_C = Car_H.next;
    while (tail_C != NULL) {
        ListInsert_T(L, tail_C->CarInfo);
        tail_C = tail_C->next;
    }

    // close files
    fclose(fpl);
    fclose(fps);
    fclose(fpc);
}

// save data into file
int SaveData(Linklist *L) {

    // open data files
    FILE *fpl, *fps, *fpc;
    if ((fpl = fopen("/tmp/.c-curriculum-design/linedb", "wb")) == NULL)
        return -1;
    if ((fps = fopen("/tmp/.c-curriculum-design/stationdb", "wb")) == NULL)
        return -1;
    if ((fpc = fopen("/tmp/.c-curriculum-design/cardb", "wb")) == NULL)
        return -1;

    // save data to three files
    FirstNode *tail_L = L->head;
    while (tail_L != NULL) {

        SecondNode *tail_S = tail_L->first_child;
        while (tail_S != NULL) {

            ThirdNode *tail_C = tail_S->first_child;
            while (tail_C != NULL) {
                fwrite(tail_C, sizeof(ThirdNode), 1, fpc);
                tail_C = tail_C->next;
            }
            fwrite(tail_S, sizeof(SecondNode), 1, fps);
            tail_S = tail_S->next;
        }
        fwrite(tail_L, sizeof(FirstNode), 1, fpl);
        tail_L = tail_L->next;
    }

    fclose(fpl);
    fclose(fps);
    fclose(fpc);
    return 0;
}

// get information
int Readline(char *buff, int pos, char *line) {
    bzero(line, LINE_SIZE);
    return readline(buff, pos, line);
}

// analyze the message and schedule the functions
void Backstage_Main(char *payload, int payload_length) {

    int pos = 0;
    char line[LINE_SIZE];
    pos = readline(payload, pos, line);
    Linklist L;
    LoadData(&L);
    
    if (strncmp(line, "ImputLine", 9) == 0) {

        Line LineInfo;
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.number, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.name, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_name, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_tel, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_mobile, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_email, line);        
        
        ListInsert_F(&L, LineInfo);

        bzero(payload, payload_length);
        strcpy(payload, "录入配送路线成功\n");
    }

    else if (strncmp(line, "ImputStation", 12) == 0) {

        Station StationInfo;
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.number, line);
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.name, line);
        pos = Readline(payload, pos, line);
        StationInfo.distance = atof(line);     
        pos = Readline(payload, pos, line);
        StationInfo.time_to_arrive = atof(line);
        pos = Readline(payload, pos, line);
        StationInfo.time_to_stay = atof(line);

        ListInsert_S(&L, StationInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "录入站点成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "ImputCar", 8) == 0) {

        Car CarInfo;
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.license_plate, line);        
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.station_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.driver_name, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.driver_mobile, line);
        pos = Readline(payload, pos, line);
        CarInfo.goods_list.total_capacity = atof(line);
        pos = Readline(payload, pos, line);
        CarInfo.goods_list.unload = atof(line);
        pos = Readline(payload, pos, line);
        CarInfo.goods_list.upload = atof(line);

        ListInsert_T(&L, CarInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "录入车辆成功\n");
        else
            strcpy(payload, L.error);
    }
    

    else if (strncmp(line, "ModifyLine", 10) == 0) {

        Line LineInfo;
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.number, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.name, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_name, line);        
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_tel, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_mobile, line);
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.principal_email, line);

        ModifyLine(&L, LineInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "修改路线信息成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "ModifyStation", 13) == 0) {

        Station StationInfo;
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.number, line);
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.name, line);
        pos = Readline(payload, pos, line);
        if (line[0] == '#')
            StationInfo.time_to_arrive = -1;
        else
            StationInfo.time_to_arrive = atof(line);
        pos = Readline(payload, pos, line);
        if (line[0] == '#')
            StationInfo.time_to_stay = -1;
        else
            StationInfo.time_to_stay = atof(line);

        ModifyStation(&L, StationInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "修改站点信息成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "ModifyCar", 9) == 0) {

        Car CarInfo;
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.license_plate, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.station_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.driver_name, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.driver_mobile, line);
        pos = Readline(payload, pos, line);
        if (line[0] == '#')
            CarInfo.goods_list.total_capacity = -1;
        else
            CarInfo.goods_list.total_capacity = atof(line);
        pos = Readline(payload, pos, line);
        if (line[0] == '#')
            CarInfo.goods_list.unload =  -1;
        else
            CarInfo.goods_list.unload = atof(line);
        pos = Readline(payload, pos, line);
        if (line[0] == '#')
            CarInfo.goods_list.upload = -1;
        else
            CarInfo.goods_list.upload = atof(line);
        ModifyCar(&L, CarInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "修改车辆信息成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "DeleteLine", 10) == 0) {

        Line LineInfo;
        pos = Readline(payload, pos, line);
        strcpy(LineInfo.number, line);

        DeleteLine(&L, LineInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "删除路线成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "DeleteStation", 13) == 0) {

        Station StationInfo;
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(StationInfo.number, line);

        DeleteStation(&L, StationInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "删除站点成功\n");
        else
            strcpy(payload, L.error);
    }

    else if (strncmp(line, "DeleteCar", 9) == 0) {

        Car CarInfo;
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.line_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.station_number, line);
        pos = Readline(payload, pos, line);
        strcpy(CarInfo.license_plate, line);

        DeleteCar(&L, CarInfo);

        bzero(payload, payload_length);
        if (strlen(L.error) == 0)
            strcpy(payload, "删除车辆成功\n");
        else
            strcpy(payload, L.error);
    }
/*
    else if (strncmp(line, "Statistics", 10) == 0) {

    }

    else if (strncmp(line, "Inquire", 7) == 0) {

    }
*/

    SaveData(&L);
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
    printf("Server working......\n\n");

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
        bzero(payload, BUFF_SIZE);

        int Ecode = receive_and_parse(connect, &head);
        if (Ecode < 0)
            continue;

        int readSize = read(connect, payload, 1024);
        if (readSize <= 0)
            continue;
        DecodeMessage(payload, readSize, head.masking_key);
        printf("Receive message from client:\n\n");
        printf("%s\n", payload);
        
        Backstage_Main(payload, sizeof(payload));
        
        SendMessage(connect, payload, sizeof(payload));

    }

    // close
    close(connect);
    close(sock_server);
    return 0;
}
