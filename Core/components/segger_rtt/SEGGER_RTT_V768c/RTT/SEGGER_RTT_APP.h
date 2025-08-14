#ifndef SEGGER_RTT_APP_H
#define SEGGER_RTT_APP_H

#include "./SEGGER_RTT.h"
#include "../Config/SEGGER_RTT_Conf.h"
#include <stdarg.h>

#define BUFFER_INDEX 0

void SEGGER_RTT_Init (void);
void print_rtt(const char * sFormat, ...);
#endif
