#include <stdarg.h>
#include <stdio.h>

#include "debug.h"
#include "const.h"

void __err_logf(const char* __file, const int __line, const char* __function, error_type err_type, const char* format, ...)
{
  #if DEBUG > 0
    va_list args;
    va_start(args, format);
    switch (err_type)
    {
      case E_OK:
        #if DEBUG > 2
        puts("[DEBUG] ");
        vprintf(format, args);
        printf(" (%s:%d:%s)\n", __file, __line, __function);
        #endif
        break;
      case E_WARN:
        #if DEBUG > 1
        puts(YELLOW"[WARNING] "RESET);
        vprintf(format, args);
        printf(" (%s:%d:%s)\n", __file, __line, __function);
        #endif
        break;

      case E_ERR:
        #if DEBUG > 0
        fputs(RED"[ERREUR]"RESET, stderr);
        vfprintf(stderr, format, args);
        fprintf(stderr, "(%s:%d:%s)\n", __file, __line, __function);
        #endif
        break;
    }
    va_end(args);
  #endif
}
