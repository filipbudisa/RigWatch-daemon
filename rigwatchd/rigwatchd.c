#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include "config.h"
#include "system/amd.h"
#include "connection.h"
#include "miner/claymore.h"


int main(int argc, char* argv[]){
	confInit();
	confCheck();

	printf("Name: %s\n", rigName);
	printf("Panel host: %s\n", panelHost);
	printf("Panel port: %s\n", panelPort);
	printf("Miner host: %s\n", minerHost);
	printf("Miner port: %s\n", minerPort);
	printf("System: %d\n", systemType);
	printf("Miner: %d\n", minerType);
	printf("\n");

	int conCode = clayResolve(minerHost, minerPort);
	if(conCode){
		printf("Error resolving miner hostname: %s\n", gai_strerror(conCode));
		exit(1);
	}

	if(!clayInit()){
		printf("Error initializing miner socker: %s\n", strerror(errno));
		exit(2);
	}

	conCode = rigConnectionInit(panelHost, panelPort);
	if(conCode){
		printf("Error initializing panel connection: %s\n", gai_strerror(conCode));
		exit(3);
	}

	printf("Connecting to panel... ");
	if(!rigConnect()){
		printf("Error: %s\n", strerror(errno));
		exit(4);
	}

	printf("Connected\n");

	printf("Registering... ");
	char* data = clayGet();

	if(data == 0){
		printf("Error fetching miner data: %s\n", strerror(errno));
		exit(5);
	}

	rigRegister(data);
	free(data);

	int code;
	uint32_t report;
	while((code = rigReceive(&report)) != -1){
		switch(code){
			case 0:
			case 1:
				data = clayGet();
				rigSend(report, data, code);
				free(data);
				break;
			case 2:
				break;
		}
	}

	printf("Panel connection lost\n");

	return 6;
}