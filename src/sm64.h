#ifndef SM64_H
#define SM64_H

// US version addresses.
#ifdef SM64_U

// Variables.
#define SM64_gDisplayListHead_tail_ptr			0x8033B06C

// Functions
#define PrintXY_addr                            0x802D66C0
#define PrintInt_addr                           0x802D62D8
#define GetSegmentBase_addr                     0x80277F20
#define SegmentedToVirtual_addr                 0x80277F50
#define alloc_displaylist_addr                  0x8019CF44
#define osInvalDCache_addr                      0x803243B0
#define funcCalledAtCleanUpDisplayListHook      0x8024784C

// Hooks.
#define SM64_RAMEntryPoint                      0x80245000
#define SM64_DMAHookCode                        0x80246050
#define SM64_DMAHookJump                        0x802789FC
#define SM64_CleanUpDisplayListHook             0x80247D1C
#define SM64_ROMPaddingStart                    0x007CC700
#define SM64_ROMMainHook                        0x0021CCE0
#define SM64_DMACopy                            0x80278504

#endif // SM64_U

// J version addresses.
#ifdef  SM64_J

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x80339CFC

// Functions
#define PrintXY_addr                            0x802D5BE0
#define PrintInt_addr                           0x802D57F8
#define GetSegmentBase_addr                     0x80277970
#define SegmentedToVirtual_addr                 0x802779A0
#define alloc_displaylist_addr                  0x8019CE04
#define osInvalDCache_addr                      0x80323480
#define funcCalledAtCleanUpDisplayListHook      0x8024781C

// Hooks.
#define SM64_RAMEntryPoint                      0x80245000
#define SM64_DMAHookCode                        0x80246050
#define SM64_DMAHookJump                        0x8027844C
#define SM64_DMACopy                            0x80277F54
#define SM64_CleanUpDisplayListHook             0x80247CEC
#define SM64_ROMPaddingStart                    0x00761BE0
#define SM64_ROMMainHook                        0x0021AFF0

#endif // SM64_J

#ifdef SM64_E

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x803092A8

#endif // SM64_E

#ifdef SM64_S

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x8031D5B4

// Functions
#define PrintXY_addr                            0x802BB908
#define PrintInt_addr                           0x802BB620
#define GetSegmentBase_addr                     0x80271830
#define SegmentedToVirtual_addr                 0x80271850
#define alloc_displaylist_addr                  0x8019C3C4
#define osInvalDCache_addr                      0x80303690
#define funcCalledAtCleanUpDisplayListHook      0x8024AD78

// Hooks.
#define SM64_RAMEntryPoint                      0x80248000
#define SM64_DMAHookCode                        0x80249050
#define SM64_DMAHookJump                        0x8027207C
#define SM64_DMACopy                            0x80271CB8
#define SM64_CleanUpDisplayListHook             0x8024B104
#define SM64_ROMPaddingStart                    0x0079D6D0
#define SM64_ROMMainHook                        0x001F7D00

#endif // SM64_S


// Assignments of variables and functions.
#define SM64_gDisplayListHead                   (*(Gfx **)  SM64_gDisplayListHead_tail_ptr)

// Function declarations.
void  (*PrintXY)           (unsigned int x, unsigned int y, const char *str);
void  (*PrintInt)          (int x, int y, const char *format, ...);
int   (*GetSegmentBase)    (int segment);
void* (*SegmentedToVirtual)(void* addr);
void* (*alloc_displaylist) (unsigned int size);
void  (*osInvalDCache)     (void* addr, int nbytes);

#endif // SM64_H