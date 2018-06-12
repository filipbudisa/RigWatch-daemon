#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <zconf.h>
#include <memory.h>
#include "claymore.h"

static struct sockaddr *addr;
static int sock;

#define rwdebug

#ifdef rwdebug

#include <time.h>
#include <stdio.h>

#endif

int clayResolve(char* address, char* port){
#ifdef rwdebug
	return 0;
#endif

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

bool clayInit(){
#ifdef rwdebug
	srand(time(0));
	return true;
#endif

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1) return false;
	return true;
}

char* clayGet(){
#ifdef rwdebug
	char debugBuf[700] = {0};
	int size = sprintf(debugBuf,
		"{\"id\": 0, \"result\": [\"11.7 - ETH\", \"3294\", \"215922;4436;0\", \"%d;%d;%d;%d;%d;%d;%d;%d\", \"0;0;0\", \"off;off;off;off;off;off;off;off\", \"%d;100;%d;100;%d;100;%d;100;%d;100;%d;80;%d;100;%d;100\", \"eth-eu1.nanopool.org:9999\", \"0;0;0;0\"], \"error\": null}",
					   (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX),
					   (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX), (int)(30000 - 1000 * (float) rand()/RAND_MAX),
					   (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX),
					   (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX), (int)(80 - 15 * (float) rand()/RAND_MAX));

	char* data = malloc((size+1)*sizeof(char));
	memset(data, 0, (size+1)*sizeof(char));
	memcpy(data, debugBuf,
		   size*sizeof(char));
	return data;
#endif

	if(connect(sock, addr, sizeof(struct sockaddr)) == -1){
		return 0;
	}

	send(sock, "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"miner_getstat1\"}", 50*sizeof(char), 0);

	char* buf = malloc(1024*sizeof(char));
	memset(buf, 0, 1024*sizeof(char));

	recv(sock, buf, 1024*sizeof(char), 0);

	close(sock);
	return buf;
}