//
// Created by filip on 01.06.18..
//

#ifndef RIGWATCH_DAEMON_CLAYMOER_H
#define RIGWATCH_DAEMON_CLAYMOER_H

int clayResolve(char* host, char* port);
char* clayGet();
bool clayCheck();
bool clayRestart();
bool clayReboot();

#endif //RIGWATCH_DAEMON_CLAYMOER_H
