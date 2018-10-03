#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "config.h"
#include "rigwatchd.h"

char *rigName = "";
char *panelHost = "";
char* panelPort = "";
System systemType;
Miner minerType;
char *minerHost = "";
char* minerPort = "";

bool reportMining = true;
bool reportSystem = true;
bool sendData = true;

void confInit(){
	FILE* confFile = fopen(CONFIG_FILE, "r"); // TODO: conf file creation
	if(confFile == 0){
		rwlog("Error opening config file %s\n", CONFIG_FILE);
		exit(22);
	}

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
		else if(strcmp(key, "panel_port") == 0){
			panelPort = (char*) malloc(strlen(val)+1);
			strcpy(panelPort, val);
		}else if(strcmp(key, "miner_host") == 0){
			minerHost = (char*) malloc(strlen(val)+1);
			strcpy(minerHost, val);
		}else if(strcmp(key, "miner_port") == 0){
			minerPort = (char*) malloc(strlen(val)+1);
			strcpy(minerPort, val);
		}else if(strcmp(key, "system") == 0){
			if(strcmp(val, "amd") == 0) systemType = AMD;
			else if(strcmp(val, "nvidia") == 0) systemType = NVIDIA;
		}else if(strcmp(key, "miner") == 0){
			if(strcmp(val, "claymore") == 0) minerType = CLAYMORE;
		}


		free(line);
		i = 0;
	}

	fclose(confFile);
}

void confCheck(){
	if(!strncmp(rigName, "", 1) || !strncmp(panelHost, "", 1) || !strncmp(panelPort, "", 1)){
		rwlog(1, "Rig name, panel host or panel port missing. Aborting\n");
		exit(3);
	}
}