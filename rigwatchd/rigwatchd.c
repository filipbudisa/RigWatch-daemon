#include <stdio.h>
#include "config.h"



int main(int argc, char* argv[]){
	confInit();
	confCheck();

	printf("Name: %s\n", rigName);
	printf("Panel host: %s\n", panelHost);
	printf("Panel port: %d\n", panelPort);
	printf("Poll interval: %d\n", pollInterval);
	printf("Send interval: %d\n", sendInterval);
	printf("Miner host: %s\n", minerHost);
	printf("Miner port: %d\n", minerPort);
	printf("System: %d\n", systemType);
	printf("Miner: %d\n", minerType);

	return 0;
}