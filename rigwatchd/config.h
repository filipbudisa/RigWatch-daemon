#ifndef RIGWATCH_DAEMON_CONFIG_H
#define RIGWATCH_DAEMON_CONFIG_H

#include <stdbool.h>

/**
 * comments start with #
 * comments cannot start mid-line
 * empty lines are ignored
 * syntax: key=val
 * no spaces around =
 * no spaces on beginning of line
 * spaces at the end of line becme part of val
 */

#define CONFIG_FILE "./rigwatchd.conf"

typedef enum {
	NOSYSYSTEM,
	AMD,
	NVIDIA
} System;

typedef enum {
	NOMINER,
	CLAYMORE
} Miner;

char *rigName;
char *panelHost;
char* panelPort;
System systemType;
Miner minerType;
char *minerHost;
char* minerPort;

bool reportMining;
bool reportSystem;
bool sendData;

void confInit();
void confCheck();

#endif //RIGWATCH_DAEMON_CONFIG_H
