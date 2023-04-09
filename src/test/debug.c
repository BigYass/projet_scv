#include <stdarg.h>
#include <stdio.h>

#include "../include/test/debug.h"
#include "../include/const.h"

void __err_logf(const char* __file, const int __line, const char* __function, error_type err_type, const char* format, ...)
{
  #if DEBUG > 0
    va_list args;
    va_start(args, format);
    switch (err_type)
    {
      case E_OK:
        #if DEBUG > 2
        printf("[DEBUG] ");
        printf("Dans la fonction %s (%s:%d) : ", __function, __file, __line);
        vprintf(format, args);
        puts("\n");
        #endif
        break;
      case E_WARN:
        #if DEBUG > 1
        printf(YELLOW"[WARNING] "RESET);
        printf("Dans la fonction %s (%s:%d) : ", __function, __file, __line);
        vprintf(format, args);
        puts("\n");
        #endif
        break;

      case E_ERR:
        #if DEBUG > 0
        fprintf(stderr, RED"[ERREUR] "RESET);
        fprintf(stderr, "Dans la fonction %s (%s:%d) : ", __function, __file, __line);
        vfprintf(stderr, format, args);
        puts("\n");
        #endif
        break;
    }
    va_end(args);
  #endif
}
