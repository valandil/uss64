#ifndef SM64_H
#define SM64_H

/// US version addresses.
#ifdef  SM64_U
// Variables.
#define SM64_gDisplayListHead_tail_ptr			0x8033B06C

// Functions
#define PrintXY_addr                            0x802D66C0
#define sm64_printf_addr                        0x802D62D8
#define GetSegmentBase_addr                     0x80277F20
#define SegmentedToVirtual_addr                 0x80277F50
#define alloc_displaylist_addr                  0x8019CF44
#define func_0x8024784C_addr                    0x8024784C

// Hooks.
#define SM64_CleanUpDisplayList_hook            0x80247D1C
#endif

/// J version addresses.
#ifdef  SM64_J

// Variables
//#define sm64_gDisplayListHead_tail_ptr

// Functions

// Hooks

#endif


/// Assignments of variables and functions.
#define SM64_gDisplayListHead                   (*(Gfx **)  SM64_gDisplayListHead_tail_ptr)

#endif // SM64_H