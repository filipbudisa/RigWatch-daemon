#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <zconf.h>
#include <signal.h>
#include "config.h"
#include "system/amd.h"
#include "connection.h"
#include "miner/claymore.h"


bool reconnect(){
	int conCode = rigConnectionInit(panelHost, panelPort);
	if(conCode){
		printf("Error initializing panel connection: %s\n", gai_strerror(conCode));
		return false;
	}

	printf("Connecting to panel... ");
	if(!rigConnect()){
		printf("Error: %s\n", strerror(errno));
		return false;
	}

	printf("Connected\n");
	return true;
}

bool work(){
	char* data;
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

	return true;
}

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

	printf("Waiting a minute for miner to come online...\n");
	sleep(60);

	while(!reconnect()){
		printf("Retrying in a minute\n");
		rigCleanup();
		sleep(60);
	}

	printf("Registering... ");
	char* data = clayGet();

	if(data == 0){
		printf("Error fetching miner data: %s\n", strerror(errno));
		exit(5);
	}

	printf("\n");

	rigRegister(data);
	free(data);

	while(work()){
		printf("Panel connection lost, reconnecting... ");

		while(!reconnect()){
			printf("Retrying in a minute\n");
			rigCleanup();
			sleep(60);
		}
	}

	return 0;
}