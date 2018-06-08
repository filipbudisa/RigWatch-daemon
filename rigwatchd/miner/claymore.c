#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <zconf.h>
#include <memory.h>
#include "claymore.h"

static struct sockaddr *addr;
static int sock;

#define rwdebug

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
	return true;
#endif

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1) return false;
	return true;
}

char* clayGet(){
#ifdef rwdebug
	char* data = malloc(268*sizeof(char));
	memcpy(data, "{\"id\": 0, \"result\": [\"11.7 - ETH\", \"3294\", \"215922;4436;0\", \"24255;29936;29935;29942;24000;29961;17949;29940\", \"0;0;0\", \"off;off;off;off;off;off;off;off\", \"76;100;75;100;76;100;71;100;77;100;66;80;76;100;75;100\", \"eth-eu1.nanopool.org:9999\", \"0;0;0;0\"], \"error\": null}",
		   268*sizeof(char));
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