// ------------------------------------------------------------------------- //
// Author:       Joey Dumont                   <joey.dumont@gmail.com>       //
// Date:         2018-01-15                                                  //
// Description:  ASM code that links the trainer code in the ROM and then    //
//               DMAs it to expansion pak memory.                            //
//                                                                           //
// We hook into the CopyScriptInterpreter function of SM64 to DMA copy our   //
// payload to expansion pak memory. We exploit the fact that the function    //
// just after the initialization function is not used to jump to that block  //
// of code from CopyScriptInterpreter. This ensures that the OS has been     //
// initialized, and that the message queues were setup properly (osInitialize//
// and SetupMessageQueues are called from Main and Thread3_Main,             //
// respectively). We use the DmaCopy function contained in SM64 to copy our  //
// payload in RAM.                                                           //
// ------------------------------------------------------------------------- //

.n64

// Paths.
USS64_BIN                   equ "{USS64_BIN}"
SM64_ROM                    equ "{SM64_ROM}.z64"
USS64_ROM                   equ "{USS64_ROM}.z64"
SM64_VERSION                equ "{SM64_VERSION}"

// Hooks and such.
SM64_RAMEntryPoint           equ {SM64_RAMEntryPoint}
SM64_DMAHookCode             equ {SM64_DMAHookCode}
SM64_DMAHookJump             equ {SM64_DMAHookJump}
SM64_ROMPadding              equ {SM64_ROMPaddingStart}
SM64_ROMMainHook             equ {SM64_ROMMainHook}
SM64_CleanUpDisplayListHook  equ {SM64_CleanUpDisplayListHook}
SM64_SoundInitHook           equ {SM64_SoundInitHook}
SM64_DMACopy                 equ {SM64_DMACopy}
SM64_osInvalDCache           equ {osInvalDCache}
USS64_DisplayAddr            equ {USS64_DisplayHook}
SM64_interaction_star_hook1  equ {SM64_interaction_star_hook1}
SM64_interaction_star_hook2  equ {SM64_interaction_star_hook2}
USS64_interaction_star_hook1 equ {USS64_Interaction_Star_Hook_1}
USS64_interaction_star_hook2 equ {USS64_Interaction_Star_Hook_2}

.open SM64_ROM, USS64_ROM, SM64_RAMEntryPoint

// Label for SM64's DMACopy function.
.definelabel DMACopy,      SM64_DMACopy

// Replace the unused space right after game initialization with our DMA code.
.org SM64_DMAHookCode
LoadNewCodeInExpRam:

// Prepare the stack.
addiu sp, sp, -0x18
sw    ra, 0x0014(sp)

// For the Shindou version, CopyScriptInterpreter has no
// useless branches. We therefore must replace a function
// call.
.if SM64_VERSION == "SM64_S"

jal SM64_osInvalDCache
nop

.endif

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

// Replace the useless branch by our hook.
.org SM64_SoundInitHook
jal 0x80400000

// Replace the call to 0x8024784C with a call to our function.
.org SM64_CleanUpDisplayListHook
jal USS64_DisplayAddr

// Hooks into interaction_star_or_key for non-stop.
.org SM64_interaction_star_hook1
jal USS64_interaction_star_hook1

.org SM64_interaction_star_hook2
jal USS64_interaction_star_hook2

// Import the payload at the end of the ROM.
.orga SM64_ROMPadding
.headersize 0
NewCodeRomStart:
.headersize 0x80400000 - orga()
NewCodeVaddrStart:
.headersize 0
.incbin USS64_BIN
NewCodeRomEnd:

.close
