// ------------------------------------------------------------------------- //
// Author:       Joey Dumont                   <joey.dumont@gmail.com>       //
// Date:         2018-01-15                                                  //
// Description:  ASM code that links the trainer code in the ROM and then    //
//               DMAs it to expansion pak memory.                            //
//                                                                           //
// We hook into the CopyScriptInterpreter (0x802789FC) function of SM64, to  //
// DMA copy our payload to expansion pak memory. We exploit the fact that the//
// function starting at 0x80246050 is not used by SM64 to jump to that block //
// of code from CopyScriptInterpreter. This ensures that the OS has been     //
// initialized, and that the message queues were setup properly (osInitalize //
// and SetupMessageQueues are called from Main and Thread3_Main,             //
// respectively). We use the DmaCopy function contained in SM64 to DMA copy  //
// our payload in RAM.                                                       //
// ------------------------------------------------------------------------- //

.n64

// Hooks and such.
SM64_RAMEntryPoint          equ {SM64_RAMEntryPoint}
SM64_DMAHookCode            equ {SM64_DMAHookCode}
SM64_DMAHookJump            equ {SM64_DMAHookJump}
SM64_ROMPadding             equ {SM64_ROMPaddingStart}
SM64_ROMMainHook            equ {SM64_ROMMainHook}
SM64_CleanUpDisplayListHook equ {SM64_CleanUpDisplayListHook}
USS64_DisplayAddr           equ {USS64_DisplayAddr}

.open "SM64.z64", "SM64-PracRom-asm.z64", SM64_RAMEntryPoint

// Labels to be placed in a separate asm file eventually.
// void DMACopy(unsigned int RAM_offset, unsigned int ROM_bottom, unsigned int ROM_top);
.definelabel DMACopy, 0x80278504

// Replace the unused space at 0x80246050 with our DMA.
.org SM64_DMAHookCode
LoadNewCodeInExpRam:

// Prepare the stack.
addiu sp, sp, -0x18
sw    ra, 0x0014(sp)

// DMA the payload to exp. pak RAM.
la    a0, NewCodeVaddrStart
la    a1, NewCodeRomStart
la    a2, NewCodeRomEnd
jal DMACopy
nop

// Restore the stack.
lw ra, 0x0014(sp)
jr ra
addiu sp, sp, 0x18

// Hijack SM64's Thread3_Main function, after the message queues
// have been initialized.
.org SM64_DMAHookJump
jal LoadNewCodeInExpRam

// Replace unused Mario behaviour with the our payload, executed at each frame.
.orga SM64_ROMMainHook
.dw 0x80400000

// Replace the call to 0x8024784C with a call to our function.
.org SM64_CleanUpDisplayListHook
jal USS64_DisplayAddr

// Import the payload at the end of the ROM.
.orga SM64_ROMPadding
.headersize 0
NewCodeRomStart:
.headersize 0x80400000 - orga()
NewCodeVaddrStart:
.headersize 0
.incbin "../uss64.bin"
NewCodeRomEnd:

.close
