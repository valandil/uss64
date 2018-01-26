// ------------------------------------------------------------------------- //
// Author:       Joey Dumont                   <joey.dumont@gmail.com>       //
// Date:         2018-01-15                                                  //
// Description:  ASM code that links the trainer code in the ROM and then    //
//               DMAs it to expansion pak memory.                            //
//                                                                           //
// We hook into the Main (0x1DF8) function of SM64, to DMA copy our payload  //
// to expansion pak memory. We exploit the fact that the function starting at//
// 0x80246050 is not used by SM64 to jump to that block of code from Main,   //
// after osInitalize has been called, to use the DmaCopy function contained  //
// in SM64 to DMA copy our payload in RAM.                                   //
// ------------------------------------------------------------------------- //

.n64

// RAM entry point is 0x80245000
ramEntryPoint equ 0x80245000
romPadding    equ 0x007CC6C0
hookPoint     equ 0x1E50

.open "SM64-PracRom.z64", "SM64-PracRom-asm.z64", ramEntryPoint

// Labels to be placed in a separate asm file eventually.
// void PrintXY(unsigned int x, unsigned int y, const char *str);
.definelabel PrintXY, 0x802D66C0

// void DmaCopy(unsigned int RAM_offset, unsigned int ROM_bottom, unsigned int ROM_top);
.definelabel DmaCopy, 0x80278504

// Import the payload at the end of the ROM.
.orga romPadding
NewCodeVaddrStart:
.importobj "../hello_world.o"
NewCodeVaddrEnd:

// Hijack the init at ROM 0x1000 function to DMA our payload in RAM.
// This should DMA from ROM 0x007CC6C0 to whatever size is 0x50 + the linked
// hello_world.o object. It should then jump back to the old initialization
// function.
.orga hookPoint
jal 0x80246050

.org 0x80246050
addiu sp, sp, 0xFFE8
sw $ra, 0x0014(sp)
la a0, 0x80400000
la a1, NewCodeVaddrStart - ramEntryPoint
la a2, NewCodeVaddrEnd - ramEntryPoint
jal DmaCopy
nop
lw ra, 0x0014(sp)
jr ra
nop
addiu sp, sp, 0x0018
nop

// replace unused Mario behavior with the our payload.
.orga 0x21CCE0
jal 0x80400000

.close
