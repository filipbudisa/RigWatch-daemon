#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "config.h"

char *rigName = "";
char *panelHost = "";
int panelPort = 80;
int pollInterval = 5;
int sendInterval = 5;
System systemType;
Miner minerType;
char *minerHost = "";
int minerPort = 0;

bool reportMining = true;
bool reportSystem = true;
bool sendData = true;

void confInit(){
	FILE* confFile = fopen(CONFIG_FILE, "r"); // TODO: conf file creation

	char *line, key[20], val[100];
	size_t i = 0;
	while((getline(&line, &i, confFile)) != -1){
		if(i == 1 || line[0] == '#'){
			//free(line);
			continue;
		}

		if(line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';

		i = strcspn(line, "=");
		strncpy(key, line, i);
		strcpy(val, line+i+1);

		key[i] = '\0';


		if(strcmp(key, "name") == 0){
			rigName = (char*) malloc(strlen(val)+1);
			strcpy(rigName, val);
		}
		else if(strcmp(key, "panel_host") == 0){
			panelHost = (char*) malloc(strlen(val)+1);
			strcpy(panelHost, val);
		}
		else if(strcmp(key, "panel_port") == 0) sscanf(val, "%d", &panelPort);
		else if(strcmp(key, "update_interval") == 0) sscanf(val, "%d", &pollInterval);
		else if(strcmp(key, "send_interval") == 0) sscanf(val, "%d", &sendInterval);
		else if(strcmp(key, "miner_host") == 0){
			minerHost = (char*) malloc(strlen(val)+1);
			strcpy(minerHost, val);
		}
		else if(strcmp(key, "miner_port") == 0) sscanf(val, "%d", &minerPort);
		else if(strcmp(key, "system") == 0){
			if(strcmp(val, "amd") == 0) systemType = AMD;
			else if(strcmp(val, "nvidia") == 0) systemType = NVIDIA;
		}else if(strcmp(key, "miner") == 0){
			if(strcmp(val, "ethminer") == 0) minerType = ETHMINER;
		}


		free(line);
		i = 0;
	}

	fclose(confFile);
}

void confCheck(){
	if(rigName == ""){
		printf("Rig name not set. Stats won't be sent to panel.\n");
		sendData = false;
	}
	if(panelHost == ""){
		printf("Panel URL not set. Stats won't be sent to panel.\n");
		sendData = false;
	}
	if(systemType == NOSYSYSTEM){
		printf("System type not set. GPU info won't be reported.\n");
		reportSystem = false;
	}
	if(minerType == NOMINER){
		printf("Miner not set. Mining stats won't be reported.\n");
		reportMining = false;
	}
	if(minerHost == ""){
		printf("Miner URL not set. Mining stats won't be reported.\n");
		reportMining = false;
	}
}