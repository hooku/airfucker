#include <stdarg.h>

#include "af_common.h"

void af_log(char* format, ...)
{
    static char ch_buffer[MAX_LOG_CHAR];

    va_list arglist;

    va_start(arglist, format);
    vsprintf(ch_buffer, format, arglist);
    va_end(arglist);

    psp_printf("%s: %s\r\n", AF_NAME, ch_buffer);
}
