#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <zconf.h>
#include <signal.h>
#include <syslog.h>
#include "rigwatchd.h"
#include "config.h"
#include "system/amd.h"
#include "connection.h"
#include "miner/claymore.h"
#include "install.h"

bool isDaemon = false;

bool reconnect(){
	int conCode = rigConnectionInit(panelHost, panelPort);
	if(conCode){
		rwlog(1, "Error initializing panel connection: %s\n", gai_strerror(conCode));
		return false;
	}

	rwlog(0, "Connecting to panel... ");
	if(!rigConnect()){
		rwlog(0, "Error: %s\n", strerror(errno));
		return false;
	}

	rwlog(0, "Connected\n");
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

				if(data == 0){
					rwlog(1, "Error fetching data from miner: %s\n", strerror(errno));
					exit(1);
				}

				rigSend(report, data, code);
				free(data);
				break;
			case 2:
				if(clayReboot()){
					exit(0);
				}else{
					rwlog(1, "Error sending reboot command\n");
				}
			case 3:
				if(!clayRestart()){
					rwlog(1, "Error sending restart command\n");
				}
		}
	}

	return true;
}

int main(int argc, char* argv[]){
	checkInstall();

	int opt;
	while((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt){
			case 'd':
				daemon(0, 0);
				isDaemon = true;
				openlog("rigwatchd", LOG_PID, LOG_USER);
				break;
			case '?':
			default:
				exit(1);
		}
	}

	confInit();
	confCheck();

	rwlog(0, "Name: %s\n", rigName);
	rwlog(0, "Panel host: %s\n", panelHost);
	rwlog(0, "Panel port: %s\n", panelPort);
	rwlog(0, "Miner host: %s\n", minerHost);
	rwlog(0, "Miner port: %s\n", minerPort);
	rwlog(0, "System: %d\n", systemType);
	rwlog(0, "Miner: %d\n", minerType);
	rwlog(0, "\n");

	int conCode = clayResolve(minerHost, minerPort);
	if(conCode){
		rwlog(1, "Error resolving miner hostname: %s\n", gai_strerror(conCode));
		exit(1);
	}

	int i = 0;
	do {
		rwlog(0, "Checking miner... ");
		if(clayCheck()){
			rwlog(0, "Online\n");
			break;
		}else{
			rwlog(0, "%d\n", i);
		}

		sleep(5);
	} while(++i < 12);

	if(i == 12){
		rwlog(1, "Couldn't connect to miner. Aborting\n");
		exit(4);
	}

	rwlog(0, "Connecting to panel...\n");
	while(!reconnect()){
		rwlog(0, "Retrying in a minute\n");
		rigCleanup();
		sleep(60);
	}

	rwlog(0, "Registering... ");
	char* data = clayGet();

	if(data == 0){
		rwlog(1, "Error fetching miner data: %s\n", strerror(errno));
		exit(5);
	}

	rwlog(0, "Registered. \n");

	rigRegister(data);
	free(data);

	while(work()){
		rwlog(1, "Panel connection lost, reconnecting... ");

		while(!reconnect()){
			rwlog(0, "Retrying in a minute\n");
			rigCleanup();
			sleep(60);
		}

		data = clayGet();
		rigRegister(data);
		free(data);
	}

	return 0;
}

void rwlog(bool important, const char* format, ...){
	if(isDaemon){
		if(!important) return;

		va_list arg;
		va_start(arg, format);
		vsyslog(LOG_INFO, format, arg);
		va_end(arg);
	}else{
		// glibc printf

		va_list arg;
		va_start(arg, format);
		vfprintf(stdout, format, arg);
		va_end(arg);
	}
}