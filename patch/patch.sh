#!/bin/bash

bash ../compile.sh
gru make-rom.lua SM64.z64
armips hook.asm
n64cksum SM64-PracRom-asm.z64
