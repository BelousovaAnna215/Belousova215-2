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

const char* buystr = "buy";
const char* nobuystr = "nobuy";
const char* sellstr = "sell";
const char* nosellstr = "nosell";

#define SERVER_IPV4_ADDR "127.0.0.1"
#define MAX_MESSAGES_SIZE 128
#define PORT 1234


typedef struct {
	int socket;// дескриптор
	struct sockaddr_in addres;// структура для подключения
} peer_t;


#endif /* MESSAGE_H */
