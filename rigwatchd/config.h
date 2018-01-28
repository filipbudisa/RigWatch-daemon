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

#define CONFIG_FILE "/home/filip/Priv/RigWatch-daemon/rigwatchd.conf"

typedef enum {
	NOSYSYSTEM,
	AMD,
	NVIDIA
} System;

typedef enum {
	NOMINER,
	ETHMINER
} Miner;

char *rigName;
char *panelHost;
int panelPort;
int pollInterval;
int sendInterval;
System systemType;
Miner minerType;
char *minerHost;
int minerPort;

bool reportMining;
bool reportSystem;
bool sendData;

void confInit();
void confCheck();

#endif //RIGWATCH_DAEMON_CONFIG_H