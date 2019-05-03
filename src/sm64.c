#include "sm64.h"

// Function declarations.
void     (*PrintXY)           (unsigned int x, unsigned int y, const char *str)= (void*)PrintXY_addr;
void     (*PrintInt)          (int x, int y, const char *format, ...)          = (void*)PrintInt_addr;
int      (*GetSegmentBase)    (int segment)                                    = (void*)GetSegmentBase_addr;
void*    (*SegmentedToVirtual)(void* addr)                                     = (void*)SegmentedToVirtual_addr;
void*    (*alloc_displaylist) (unsigned int size)                              = (void*)alloc_displaylist_addr;
void     (*osInvalDCache)     (void* addr, int nbytes)                         = (void*)osInvalDCache_addr;
uint32_t (*osGetCount)        ()                                               = (uint32_t*)osGetCount_addr;
void     (*func_8024924C)     (int16_t)                                        = (void*)func_8024924C_addr;
uint32_t (*set_mario_action)  ()                                               = (uint32_t*)set_mario_action_addr;