#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static void dLog(const char *fmt,...)
{
    char *p = NULL;
    int size = 0;
    va_list args;
                
    va_start(args,fmt);
    size = vsnprintf(p,size,fmt,args);
    va_end(args);
                    
    p = (char*)malloc(++size);
    va_start(args,fmt);
    size = vsnprintf(p,size,fmt,args);
    va_end(args);
                
    printf(p);
    free(p); 
}

#ifdef DEBUG
#define dPrint(fmt,args...) \
   dLog(fmt,##args)
#else
#define dPrint(fmt,...)
#endif