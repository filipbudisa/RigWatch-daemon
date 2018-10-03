#ifndef RIGWATCH_DAEMON_RIGWATCHD_H
#define RIGWATCH_DAEMON_RIGWATCHD_H

#include <stdbool.h>

void rwlog(bool important, const char* format, ...);
bool isDaemon;

#endif //RIGWATCH_DAEMON_RIGWATCHD_H
