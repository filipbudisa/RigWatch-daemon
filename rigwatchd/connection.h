//
// Created by filip on 02.06.18..
//

#ifndef RIGWATCH_DAEMON_CONNECTION_H
#define RIGWATCH_DAEMON_CONNECTION_H

int rigConnectionInit(char* host, char* port);
bool rigConnect();
bool rigSend(uint32_t report, char* data);
bool rigRegister(char* data);
int rigReceive(uint32_t *report);

#endif //RIGWATCH_DAEMON_CONNECTION_H
