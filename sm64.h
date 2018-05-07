#ifndef SM64_H
#define SM64_H

// Variables.
#ifdef  SM64_U
#define SM64_gDisplayListHead_tail_ptr			0x8033B06C
#define SM64_CleanUpDisplayList_hook            0x80247D1C
#endif


#ifdef  SM64_J
//#define sm64_gDisplayListHead_tail_ptr 
#endif

#define sm64_gDisplayListHead         			(*(Gfx **)	sm64_gDisplayListHead_tail_ptr)

#endif // SM64_H