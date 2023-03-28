#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

const char* buystr = "buy";
const char* nobuystr = "nobuy";
const char* sellstr = "sell";
const char* nosellstr = "nosell";
const char* exstr = "exit";

#define SERVER_IPV4_ADDR "127.0.0.1"
#define MAX_MESSAGES_SIZE 128
#define PORT 1234
#define SELL 4
#define BUY 3
#define EXIT 3


typedef struct {
	int socket;
	struct sockaddr_in addres;
} peer_t;


#endif /* MESSAGE_H */
