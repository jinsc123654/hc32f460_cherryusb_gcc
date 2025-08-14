#include "SEGGER_RTT_APP.h"

//SEGGER_RTT_Init();
//#define log_dbg(fmt,arg...)   print_rtt("[%s] " fmt "\r\n", __func__,##arg)
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

void print_rtt(const char * sFormat, ...)
{    
    va_list ParamList;
    va_start(ParamList, sFormat);
    SEGGER_RTT_vprintf(BUFFER_INDEX, sFormat, &ParamList);
    va_end(ParamList);
}
