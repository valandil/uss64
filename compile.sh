cd $(dirname $(readlink -f $0))
BIN_ROOT=/opt/n64/bin/
N64_SYSROOT=/opt/n64/mips64/n64-sysroot/
CFLAGS='-g -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -DF3D_GBI -DSM64_U'
$BIN_ROOT/grc gz/res/gz/fipps.png -d gz/res/resources.json -o fipps.png.o
$BIN_ROOT/mips64-gcc                         $CFLAGS -c                             $N64_SYSROOT/usr/include/grc.c           -o grc.o
$BIN_ROOT/mips64-gcc                         $CFLAGS -c                             $N64_SYSROOT/usr/include/vector/vector.c -o vector.o
$BIN_ROOT/mips64-gcc                         $CFLAGS -c                             $N64_SYSROOT/usr/include/startup.c       -o startup.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include uss64.c                                  -o uss64.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include sm64.c                                   -o sm64.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include gz/src/gz/resource.c                     -o resource.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include gz/src/gz/gfx.c                          -o gfx.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include gz/src/gz/gu.c                           -o gu.o
$BIN_ROOT/mips64-gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I $N64_SYSROOT/usr/include gz/src/gz/zu.c                           -o zu.o
$BIN_ROOT/mips64-g++                                                         \
           -Wa,-adhln                                                        \
           -T /opt/n64/mips64/n64-sysroot/usr/lib/gl-n64.ld                  \
           -nostartfiles                                                     \
           -specs=nosys.specs                                                \
           -O3                                                               \
           -flto                                                             \
           -Wl,--gc-sections                                                 \
           -Wl,--defsym,start=0x80400000                                     \
           -mabi=32                                                          \
           uss64.o sm64.o resource.o gfx.o gu.o grc.o zu.o vector.o startup.o \
           -o uss64.elf
$BIN_ROOT/mips64-objcopy -O binary uss64.elf uss64.bin
