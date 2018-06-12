
#include <netdb.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

static struct sockaddr *addr;
static int sock;

int rigConnectionInit(char* address, char* port){
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	struct addrinfo *res;
	int code = getaddrinfo(address, port, &hints, &res);
	if(code) return code;

	addr = res->ai_addr;
	return 0;
}

bool rigConnect(){
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		return false;
	}

	if(connect(sock, addr, sizeof(struct sockaddr)) == -1){
		return false;
	}

	return true;
}

bool rSend(char* data, size_t len){
	if(send(sock, data, len, 0) == -1){
		free(data);
		return false;
	}

	free(data);
	return true;
}

bool rigSend(uint32_t report, char* data, uint8_t action){
	size_t len = strlen(data);

	char preData[500];
	size_t preDataLen;

	if(action == 0){
		preDataLen = sprintf(preData, "report;%s;%u", rigName, report);
	}else{
		preDataLen = sprintf(preData, "data;%s;%u", rigName, report);
	}

	char* buf = malloc((len+preDataLen)*sizeof(char));
	memset(buf, 0, (len+preDataLen)*sizeof(char));
	memcpy(buf, preData, preDataLen*sizeof(char));
	memcpy(buf+preDataLen, data, len);

	return rSend(buf, (len+preDataLen)*sizeof(char));
}

bool rigRegister(char* data){
	size_t len = strlen(data);
	size_t nameLen = strlen(rigName);

	char* buf = malloc((4+len+nameLen)*sizeof(char));
	memset(buf, 0, (4+len+nameLen)*sizeof(char));
	memcpy(buf, "reg;", 4*sizeof(char));
	memcpy(buf+4, rigName, nameLen);
	memcpy(buf+nameLen+4, data, len);

	return rSend(buf, (4+len+nameLen)*sizeof(char));
}

int rigReceive(uint32_t *report){
	uint8_t buf;

	if(recv(sock, &buf, sizeof(uint8_t), 0) <= 0){
		return -1;
	}

	uint32_t *buf2;
	if(report == 0) buf2 = malloc(sizeof(uint32_t));
	else buf2 = report;

	if(recv(sock, buf2, sizeof(uint32_t), 0) <= 0){
		return -1;
	}

	if(report == 0) free(buf2);

	return buf;
}