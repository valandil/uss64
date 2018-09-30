#include "sm64.h"

// Function declarations.
void  (*PrintXY)           (unsigned int x, unsigned int y, const char *str)= (void*)PrintXY_addr;
void  (*PrintInt)          (int x, int y, const char *format, ...)          = (void*)PrintInt_addr;
int   (*GetSegmentBase)    (int segment)                                    = (void*)GetSegmentBase_addr;
void* (*SegmentedToVirtual)(void* addr)                                     = (void*)SegmentedToVirtual_addr;
void* (*alloc_displaylist) (unsigned int size)                              = (void*)alloc_displaylist_addr;
void  (*osInvalDCache)     (void* addr, int nbytes)                         = (void*)osInvalDCache_addr;
