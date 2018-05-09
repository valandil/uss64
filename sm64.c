#include "sm64.h"

// Function declarations.
void  (*PrintXY)           (unsigned int x, unsigned int y, const char *str)= (void*)PrintXY_addr
void  (*sm64_printf)       (int x, int y, const char *format, ...)          = (void*)sm64_printf_addr
int   (*GetSegmentBase)    (int segment)                                    = (void*)GetSegmentBase_addr
void* (*SegmentedToVirtual)(void* addr)                                     = (void*)SegmentedToVirtual_addr
void* (*alloc_displaylist) (unsigned int size)                              = (void*)alloc_displaylist_addr
void  (*func_0x8024784C)   ()                                               = (void*)func_0x8024784C_addr
