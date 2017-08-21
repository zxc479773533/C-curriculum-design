/*
* Part: Backstage server
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

// the key used to build conncetion
#define KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


/* struct definations */



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

extern void DecodeMessage(char *data, int len, char *mask);
/*
* Function Name: shakehands
* Module: Server
* Parameter: int sock_client
* Return: int
* Use: shakehands with client and build connection
*/ 







#endif // !__WSSERVER_H_