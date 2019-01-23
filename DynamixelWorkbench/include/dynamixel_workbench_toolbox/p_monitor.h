#ifndef P_MONITOR_H
#define P_MONITOR_H

#include "WString.h"

void dynamixel_command(String cmd[]);
void split(String data, char separator, String* temp);

#endif //P_MONITOR
