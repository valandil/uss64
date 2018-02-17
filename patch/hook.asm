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

// Useful constants. Should be placed in another file
// and modified for other versions of the game.
ramEntryPoint equ 0x80245000
dmaHook       equ 0x80246050
romPadding    equ 0x007CC700
hookPoint     equ 0x802789FC
behaviourHook equ 0x0021CCE0

.open "SM64.z64", "SM64-PracRom-asm.z64", ramEntryPoint

// Labels to be placed in a separate asm file eventually.
// void PrintXY(unsigned int x, unsigned int y, const char *str);
.definelabel PrintXY, 0x802D66C0

// void DmaCopy(unsigned int RAM_offset, unsigned int ROM_bottom, unsigned int ROM_top);
.definelabel DmaCopy, 0x80278504

// Replace the unused space at 0x80246050 with our DMA.
.org dmaHook
LoadNewCodeInExpRam:

// Prepare the stack.
addiu sp, sp, -0x18
sw    ra, 0x0014(sp)

// DMA the payload to exp. pak RAM.
la    a0, NewCodeVaddrStart
la    a1, NewCodeRomStart
la    a2, NewCodeRomEnd
jal DmaCopy
nop

// Restore the stack.
lw ra, 0x0014(sp)
jr ra
addiu sp, sp, 0x18


// Hijack SM64's Thread3_Main function, after the message queues
// have been initialized.
.org hookPoint
jal LoadNewCodeInExpRam

// Replace unused Mario behaviour with the our payload, executed at each frame.
.orga behaviourHook
.dw 0x80400000

// Import the payload at the end of the ROM.
.orga romPadding
.headersize 0
NewCodeRomStart:
.headersize 0x80400000 - orga()
NewCodeVaddrStart:
.align 4
.headersize 0
.incbin "../uss64.bin"
.headersize 0
NewCodeRomEnd:

.close
