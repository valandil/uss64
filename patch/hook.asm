// ------------------------------------------------------------------------- //
// Author:       Joey Dumont                   <joey.dumont@gmail.com>       //
// Date:         2018-01-15                                                  //
// Description:  ASM code that links the trainer code in the ROM and then    //
//               DMAs it and the SM64 initialization function to expansion   //
//               pak memory.                                                 //
//                                                                           //
// We assume that the old initialization function at 0x1000 was already      //
// copied to 0x007C6C0 by the make-rom.lua LUA script. We then overwrite the //
// init function at 0x1000 by our own asm that DMAs the old init function and//
// our own trainer code to the expansion pak memory. After the DMA, we jump  //
// back to the initialization function that we DMA'd at 0x80400000.          //
// ------------------------------------------------------------------------- //

.n64
.open "SM64-PracRom.z64", "SM64-PracRom-asm.z64", 0x80245000

// void PrintXY(unsigned int x, unsigned int y, const char *str);
.definelabel PrintXY, 0x802D66C0

// void DmaCopy(unsigned int RAM_offset, unsigned int ROM_bottom, unsigned int ROM_top);
.definelabel DmaCopy, 0x80278504

// Import hello_world at the end of the ROM.
.orga 0x007CC6C0 + 0x50
NewCodeVaddrStart:
.importobj "../hello_world.o"
NewCodeVaddrEnd:

// Hijack the init at ROM 0x1000 function to DMA our payload in RAM.
// This should DMA from ROM 0x007CC6C0 to whatever size is 0x50 + the linked
// hello_world.o object. It should then jump back to the old initialization
// function.
.orga 0x1000
la a0, 0x80400000
la a1, NewCodeVaddrStart - 0x50
la a2, NewCodeVaddrEnd
jal DmaCopy
nop
j 0x80400000

// replace unused Mario behavior with the our payload.
.orga 0x21CCE0
.dw 0x80400050

.close

// Other place to execute DMA:
// 0x514.
// The culprit here seems to be jal DmaCopy, which doesn't terminate or something.
// Should check it out in BizHawk.