#!/bin/bash
bash ../compile.sh
python ../GenerateHooks.py obj/uss64.elf U
armips hook_U.asm
n64cksum SM64-PracRom-asm.z64
cp ../debug_scripts_out/uss64* /c/Users/Joey/Documents/VGs/Emulation/Project64d/Scripts
