#ifndef SM64_H
#define SM64_H

#include "input.h"
#include "types.h"

// Japanese version addresses.
#ifdef  SM64_J

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x80339CFC
#define SM64_gPlayer1Controller_addr            0x8032C6A4
#define SM64_gPlayer2Controller_addr            0x8032C6AC
#define SM64_gMarioStates_addr                  0x80339E00
#define SM64_gMarioState_addr                   0x8032c9d8
#define SM64_sNumVblanks_addr                   0x8032C640
#define SM64_gSpecialTripleJump_addr            0x8032CE34
#define SM64_gCurrLevelNum_addr                 0x8032CE98
#define SM64_sCurrPlayMode_addr                 0x80339ec8
#define SM64_sCurrWarpType_addr                 0x80339ed8

// Functions
#define PrintXY_addr                            0x802D5BE0
#define PrintInt_addr                           0x802D57F8
#define GetSegmentBase_addr                     0x80277970
#define SegmentedToVirtual_addr                 0x802779A0
#define alloc_displaylist_addr                  0x8019CE04
#define osInvalDCache_addr                      0x80323480
#define funcCalledAtCleanUpDisplayListHook      0x8024781C
#define osGetCount_addr                         0x80327460
#define func_8024924C_addr                      0x8024924C
#define set_mario_action_addr                   0x80252AC8
#define func_8024975C_addr                      0x8024975C
#define reset_dialog_state_addr                 0x802d8098
#define set_play_mode_addr                      0x80249734
#define initiate_warp_addr                      0x8024a594

// Hooks.
#define SM64_RAMEntryPoint                      0x80245000
#define SM64_DMAHookCode                        0x80246050
#define SM64_DMAHookJump                        0x8027844C
#define SM64_DMACopy                            0x80277F54
#define SM64_CleanUpDisplayListHook             0x80247CEC
#define SM64_ROMPaddingStart                    0x00761BE0
#define SM64_ROMMainHook                        0x0021AFF0
#define SM64_SoundInitHook                      0x802494B8
#define SM64_interaction_star_hook1             0x8024DC1C
#define SM64_interaction_star_hook2             0x8024DC74

#endif // SM64_J

// US version addresses.
#ifdef SM64_U

// Variables.
#define SM64_gDisplayListHead_tail_ptr			    0x8033B06C
#define SM64_gPlayer1Controller_addr            0x8032D5E4
#define SM64_gPlayer2Controller_addr            0x8032D5EC
#define SM64_gMarioStates_addr                  0x8033B170
#define SM64_gMarioState_addr                   0x8032d93c
#define SM64_sNumVblanks_addr                   0x8032D580
#define SM64_gSpecialTripleJump_addr            0x8032DD94
#define SM64_gCurrLevelNum_addr                 0x8032DDF8
#define SM64_sCurrPlayMode_addr                 0x8033b238
#define SM64_sCurrWarpType_addr                 0x8033b248

// Functions
#define PrintXY_addr                            0x802D66C0
#define PrintInt_addr                           0x802D62D8
#define GetSegmentBase_addr                     0x80277F20
#define SegmentedToVirtual_addr                 0x80277F50
#define alloc_displaylist_addr                  0x8019CF44
#define osInvalDCache_addr                      0x803243B0
#define funcCalledAtCleanUpDisplayListHook      0x8024784C
#define osGetCount_addr                         0x803283E0
#define func_8024924C_addr                      0x8024927C
#define set_mario_action_addr                   0x80252CF4
#define func_8024975C_addr                      0x8024978C
#define reset_dialog_state_addr                 0x802d8d90
#define set_play_mode_addr                      0x80249764
#define initiate_warp_addr                      0x8024a700

// Hooks.
#define SM64_RAMEntryPoint                      0x80245000
#define SM64_DMAHookCode                        0x80246050
#define SM64_DMAHookJump                        0x802789FC
#define SM64_CleanUpDisplayListHook             0x80247D1C
#define SM64_ROMPaddingStart                    0x007CC700
#define SM64_ROMMainHook                        0x0021CCE0
#define SM64_DMACopy                            0x80278504
#define SM64_SoundInitHook                      0x802494E8
#define SM64_interaction_star_hook1             0x8024DDBC
#define SM64_interaction_star_hook2             0x8024DE1C

#endif // SM64_U

// Shindou version addresses.
#ifdef SM64_S

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x8031D5B4
#define SM64_gPlayer1Controller_addr            0x8030CD14
#define SM64_gPlayer2Controller_addr            0x8030CD1C
#define SM64_gMarioStates_addr                  0x8031D9C0
#define SM64_gMarioState_addr                   0x8030D068
#define SM64_sNumVblanks_addr                   0x8030CCB0
#define SM64_gSpecialTripleJump_addr            0x8030D4C4
#define SM64_gCurrLevelNum_addr                 0x8030D528
#define SM64_sCurrPlayMode_addr                 0x8031DA96
#define SM64_sCurrWarpType_addr                 0x8031DAA0

// Functions
#define PrintXY_addr                            0x802BB908
#define PrintInt_addr                           0x802BB620
#define GetSegmentBase_addr                     0x80271830
#define SegmentedToVirtual_addr                 0x80271850
#define alloc_displaylist_addr                  0x8019C3C4
#define osInvalDCache_addr                      0x80303690
#define funcCalledAtCleanUpDisplayListHook      0x8024AD78
#define osGetCount_addr                         0x80304FB0
#define func_8024924C_addr                      0x8024C1BC
#define set_mario_action_addr                   0x80254298
#define func_8024975C_addr                      0x8024CDF4
#define reset_dialog_state_addr                 0x802BD4F0
#define set_play_mode_addr                      0x8024CDDC
#define initiate_warp_addr                      0x8024DAE0

// Hooks.
#define SM64_RAMEntryPoint                      0x80248000
#define SM64_DMAHookCode                        0x80249050
#define SM64_DMAHookJump                        0x8027207C
#define SM64_DMACopy                            0x80271CB8
#define SM64_CleanUpDisplayListHook             0x8024B104
#define SM64_ROMPaddingStart                    0x0079D6D0
#define SM64_ROMMainHook                        0x001F7D00
#define SM64_SoundInitHook                      0x8024C39C
#define SM64_FuncCalledAtSoundInitHook          0x802FD6B0
#define SM64_interaction_star_hook1             0x80250634
#define SM64_interaction_star_hook2             0x80250688

#endif // SM64_S

// iQue version addresses.
#ifdef SM64_Q

#endif // SM64_Q

// PAL version addresses.
#ifdef SM64_E

// Variables.
#define SM64_gDisplayListHead_tail_ptr          0x803092A8

#endif // SM64_E

// 64DD version addresses.
#ifdef SM64_D

#endif // SM64_D

// Constants valid for all versions.
#define ACT_FALL_AFTER_STAR_GRAB                0x00001904
#define ACT_STAR_DANCE_EXIT                     0x00001302
#define ACT_STAR_DANCE_NO_EXIT                  0x00001307

// Assignments of variables and functions.
#define SM64_gDisplayListHead                   (*(Gfx **)         SM64_gDisplayListHead_tail_ptr)
#define SM64_gPlayer1Controller                 (*(Controller **)  SM64_gPlayer1Controller_addr)
#define SM64_gPlayer2Controller                 (*(Controller **)  SM64_gPlayer2Controller_addr)
#define SM64_gMarioState                        (*(struct MarioState **)  SM64_gMarioState_addr)
#define SM64_sNumVblanks                        (*(uint32_t*)      SM64_sNumVblanks_addr)
#define SM64_gSpecialTripleJump                 (*(uint8_t*)       SM64_gSpecialTripleJump_addr)
#define SM64_gCurrLevelNum                      (*(int16_t*)       SM64_gCurrLevelNum_addr)
#define SM64_sCurrPlayMode                      (*(int16_t*)       SM64_sCurrPlayMode_addr)
#define SM64_sCurrWarpType                      (*(uint8_t*)       SM64_sCurrWarpType_addr)

// Function declarations.
void       (*PrintXY)           (unsigned int x, unsigned int y, const char *str);
void       (*PrintInt)          (int x, int y, const char *format, ...);
int        (*GetSegmentBase)    (int segment);
void*      (*SegmentedToVirtual)(void* addr);
void*      (*alloc_displaylist) (unsigned int size);
void       (*osInvalDCache)     (void* addr, int nbytes);
uint32_t   (*osGetCount)        (void);
void       (*func_8024924C)     (int16_t);
uint32_t   (*set_mario_action)  ();
void       (*func_8024975C)     (int32_t);
void       (*reset_dialog_state)(void);
void       (*set_play_mode)     (int32_t);
void       (*initiate_warp)     (int16_t,int16_t,int16_t,int32_t);

#endif // SM64_H