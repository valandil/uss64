#!/bin/bash
bash ../compile.sh
python ../GenerateHooks.py uss64.elf U
armips hook_U.asm
n64cksum SM64-PracRom-asm.z64
