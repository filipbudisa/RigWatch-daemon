#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <sys/stat.h>

#include "install.h"
#include "whereami.h"
#include "config.h"

void placeScript(FILE* fp);

void checkInstall(){
	if(access(RW_INSTALL, F_OK || X_OK) == 0) return;
	printf("Binary missing, installing now.\n");

	int length = wai_getExecutablePath(NULL, 0, NULL);
	char *path = (char*) malloc(length + 1);
	wai_getExecutablePath(path, length, NULL);
	path[length] = '\0';

	FILE *dest = fopen(RW_INSTALL, "w");
	if(dest == 0){
		printf("Failed opening destination %s for writing: %s\n", RW_INSTALL, strerror(errno));
		exit(1);
	}

	FILE *src = fopen(path, "r");
	if(src == 0){
		printf("Failed opening source %s for reading: %s\n", RW_INSTALL, strerror(errno));
		exit(1);
	}

	char buf[1024];
	size_t bytes;

	while((bytes = fread(buf, 1, 1024, src)) != 0) fwrite(buf, 1, bytes, dest);

	fclose(src);
	fclose(dest);

	chmod(RW_INSTALL, S_IREAD | S_IWUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);

	char name[200] = "";
	char panelhost[200] = "";
	char panelport[200] = "";
	char minerhost[200] = "localhost\n";
	char minerport[200] = "3333\n";

	int writeConfig = 1;

	if(access(CONFIG_FILE, F_OK || R_OK) != 0){
		writeConfig = 0;

		printf("Config file exists, use it? ");
		char r[3];

		do {
			printf("(y/n) ");
			scanf("%s", r);
		} while(strncmp(r, "y", 3) && strncmp(r, "n", 3));

		writeConfig = !strncmp(r, "n", 3);
	}

	int run = 1;

	if(writeConfig){
		fgetc(stdin);

		printf("Rig unique name: ");
		fgets(name, 199, stdin);

		printf("Panel hostname: ");
		fgets(panelhost, 199, stdin);
		printf("Panel port: ");
		fgets(panelport, 199, stdin);

		char tmpminerhost[200] = "", tmpminerport[200] = "";
		printf("Miner API hostname [localhost]: ");
		fgets(tmpminerhost, 199, stdin);
		printf("Miner API port [3333]: ");
		fgets(tmpminerport, 199, stdin);

		if(strncmp(tmpminerhost, "\n", 200)) strncpy(minerhost, tmpminerhost, 200);
		if(strncmp(tmpminerport, "\n", 200)) strncpy(minerport, tmpminerport, 200);

		FILE *cfg = fopen(CONFIG_FILE, "w");
		if(cfg == 0){
			printf("Failed opening config file %s for writing: %s\n", CONFIG_FILE, strerror(errno));
			exit(1);
		}

		fprintf(cfg, "name=%s", name);
		fprintf(cfg, "panel_host=%s", panelhost);
		fprintf(cfg, "panel_port=%s", panelport);
		fprintf(cfg, "miner_host=%s", minerhost);
		fprintf(cfg, "miner_port=%s", minerport);

		fclose(cfg);

		if(!strncmp(name, "\n", 200) || !strncmp(panelhost, "\n", 200) || !strncmp(panelport, "\n", 200)) run = 0;
	}

	FILE *script = fopen(RW_SCRIPT, "w");
	if(script == 0){
		printf("Failed opening script file %s for writing: %s\n", RW_SCRIPT, strerror(errno));
		exit(1);
	}

	placeScript(script);
	fclose(script);
	chmod(RW_SCRIPT, S_IREAD | S_IWUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);

	symlink(RW_SCRIPT, "/etc/rc2.d/S01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc3.d/S01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc4.d/S01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc5.d/S01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc0.d/K01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc1.d/K01rigwatchd");
	symlink(RW_SCRIPT, "/etc/rc6.d/K01rigwatchd");

	printf("Service succesfully installed.\n");

	if(!run){
		printf("Service wont be started: rig name, panel hostname or panel port not specified.\n");
		printf("You can set these settings manually at %s\n", CONFIG_FILE);
		exit(0);
	}
}

void placeScript(FILE* fp){
	fprintf(fp, "DAEMON=%s\n"
		   "NAME=rigwatchd\n"
		   "\n"
		   "test -x $DAEMON || exit 0\n"
		   "\n"
		   "case \"$1\" in\n"
		   "    start)\n"
		   "    start-stop-daemon --start --exec $DAEMON -- \"-d\"\n"
		   "    ;;\n"
		   "\n"
		   "    stop)\n"
		   "    start-stop-daemon --stop --oknodo --retry 30 --exec $DAEMON\n"
		   "    ;;\n"
		   "\n"
		   "    restart)\n"
		   "    start-stop-daemon --stop --oknodo --retry 30 --exec $DAEMON\n"
		   "    start-stop-daemon --start --exec $DAEMON -- \"-d\"\n"
		   "    ;;\n"
		   "\n"
		   "    *)\n"
		   "    echo \"Usage: $0 {start|stop|restart}\"\n"
		   "    exit 1\n"
		   "esac\n"
		   "exit 0\n", RW_INSTALL);
}