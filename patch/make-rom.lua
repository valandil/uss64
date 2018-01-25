-- ------------------------------------------------------------------------- --
-- Author:       Joey Dumont                   <joey.dumont@gmail.com>       --
-- Date:         2018-01-15                                                  --
-- Description:  Lua script that inserts the prac ROM payload.               --
-- ------------------------------------------------------------------------- --

-- Parse command-line arguments.
local arg = {...}

-- Load the ROM with gru.
local rom = gru.n64rom_load(arg[1])

-- Load the ROM.
local prac_rom = gru.blob_load("../hello_world.o")

-- Determine the variables for the custom loader.
local payload_rom  = 0x007CC6C0
local payload_ram  = 0x80400000
--local payload_size = prac_rom:size()

--local _,_,make_result = os.execute(string.format("mips64-gcc -c " ..
--                                                 "-DDMA_ROM=0x%08X -DDMA_RAM=0x%08X -DDMA_SIZE=0x%08X " ..
--                                                 "-I /opt/n64-dev/n64-sysroot/include " ..
--                                                 "../src/ldr/ldr.c -o ldr.o",
--                                                 payload_rom, payload_ram, payload_size))
--local _,_,make_result = os.execute(string.format("mips64-g++ -T /opt/n64-dev/n64-sysroot/lib/gl-n64.ld -nostdlib -Wl,--defsym,start=0x80400000 ldr.o -o ldr.elf"))
--local _,_,make_result = os.execute(string.format("mips64-objcopy ldr.elf ldr.bin -O binary"))


-- Copy the old init function and insert the payload.
--local ldr     = gru.blob_load("ldr.bin")
local old_ldr = rom:copy(0x1000, 0x50)
--rom:write(0x1000, ldr)
rom:write(payload_rom, old_ldr)
--rom:write(payload_rom + 0x50, prac_rom)
rom:crc_update()
rom:save_file("SM64-PracRom.z64")