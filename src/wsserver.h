/*
* Part: Backstage and server
*
* Server head file
*
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __WSSERVER_H_
#define __WSSERVER_H_

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


/* defines */

// the socket queue length
#define QUEUE 20

// the port that server use
#define PORT 8080

// the max message size receive from client
#define BUFF_SIZE 1024

// the line size
#define LINE_SIZE 256

// the key used to build conncetion
#define KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


/* struct definations */

/*-------------------------------------------------------------------
0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
--------------------------------------------------------------------*/

typedef struct {
    u_char fin;
    u_char opencode;
    u_char mask;
    u_long payload_length;
    u_char masking_key[4];
} websocket_head;


/* functions */

extern int base64_encode(char *in_str, int in_len, char *out_str);
/*
* Function Name: base64_encode
* Module: Server
* Parameter: char *in_str, int in_len, char *out_str
* Return: int
* Use: Base64 encode function, used to build connection
*/

extern int readline(char *buff, int pos, char *line);
/*
* Function Name: readline
* Module: Server
* Parameter: char *buf, int pos, char *line
* Return: int
* Use: read a line string form buffer
*/

extern int shakehands(int sock_client);
/*
* Function Name: shakehands
* Module: Server
* Parameter: int sock_client
* Return: int
* Use: shakehands with client and build connection
*/

extern void strreverse(char *str, int len);
/*
* Function Name: strreverse
* Module: Server
* Parameter: char *str, int len
* Return: None
* Use: reverse string
*/

extern int receive_and_parse(int sock, websocket_head *pWS);
/*
* Function Name: receive_and_parse
* Module: Server
* Parameter: int sock, websocket_head *pWS
* Return: int
* Use: receive message from client and parse it
*/

extern void DecodeMessage(char *data, int len, u_char *mask);
/*
* Function Name: DecodeMessage
* Module: Server
* Parameter: char *data, int len, u_char *mask
* Return: None
* Use: decode message
*/

extern int SendMessage(int fd, char *payload, int payload_length);
/*
* Function Name: SendMessage
* Module: Server
* Parameter: int fd, char *payload, int payload_length
* Return: int
* Use: send message to client
*/

extern void LoadData(Linklist *L);
/*
* Function Name: LoadData
* Module: Server
* Parameter: Linklist *L
* Return: None
* Use: load data from file
*/

extern int SaveData(Linklist *L);
/*
* Function Name: SaveData
* Module: Server
* Parameter: Linklist *L
* Return: int
* Use: save data into file
*/

extern void Backstage_Main(char *payload, int payload_length);
/*
* Function Name: Backstage_Main
* Module: Server
* Parameter: char *payload, int payload_len
* Return: Node
* Use: analyze the message and schedule the functions
*/


#endif // !__WSSERVER_H_