cd $(dirname $(readlink -f $0))
grc gz/res/gz/fipps.png -d gz/res/resources.json -o fipps.png.o
mips64-gcc -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c /opt/n64-dev/n64-sysroot/include/grc.c -o grc.o
mips64-gcc -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c /opt/n64-dev/n64-sysroot/include/vector/vector.c -o vector.o
mips64-gcc -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c /opt/n64-dev/n64-sysroot/include/startup.c -o startup.o
mips64-gcc -DZ64_VERSION=Z64_OOT10 -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include hello_world.c         -o hello_world.o
mips64-gcc -DZ64_VERSION=Z64_OOT10 -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include gz/src/gz/resource.c  -o resource.o
mips64-gcc -DZ64_VERSION=Z64_OOT10 -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include gz/src/gz/gfx.c       -o gfx.o
mips64-gcc -DZ64_VERSION=Z64_OOT10 -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include gz/src/gz/gu.c        -o gu.o
mips64-gcc -DZ64_VERSION=Z64_OOT10 -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include gz/src/gz/zu.c        -o zu.o
mips64-g++ -T /opt/n64-dev/n64-sysroot/lib/gl-n64.ld -nostartfiles -specs=nosys.specs -O3 -flto -Wl,--gc-sections -Wl,--defsym,start=0x80400000 -mabi=32 \
              hello_world.o resource.o gfx.o gu.o grc.o zu.o vector.o startup.o -o uss64.o
mips64-objcopy -O binary uss64.elf uss64.bin
