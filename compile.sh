cd $(dirname $(readlink -f $0))
BIN_ROOT=/opt/n64/bin/
#BIN_ROOT=/opt/n64-dev/usr/bin
#BIN_ROOT=/usr/bin/
N64_SYSROOT=/opt/n64/mips64/n64-sysroot/usr
#N64_SYSROOT=/opt/n64-dev/mips64/n64-sysroot/usr
CFLAGS='-g -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -DF3D_GBI -DSM64_U'
CROSS=mips64-
$BIN_ROOT/grc  gz/res/gz/fipps.png -d gz/res/resources.json -o fipps.png.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                         ${N64_SYSROOT}/include/grc.c                   -o grc.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                         ${N64_SYSROOT}/include/vector/vector.c         -o vector.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                         ${N64_SYSROOT}/include/startup.c               -o startup.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include uss64.c                                      -o uss64.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include sm64.c                                       -o sm64.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/resource.c                         -o resource.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/gfx.c                              -o gfx.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/gu.c                               -o gu.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/zu.c                               -o zu.o
$BIN_ROOT/${CROSS}g++                                                         \
           -Wa,-adhln                                                         \
           -T ${N64_SYSROOT}/lib/gl-n64.ld                                    \
           -nostartfiles                                                      \
           -specs=nosys.specs                                                 \
           -O2                                                                \
           -Wall                                                              \
           -mtune=vr4300 -march=4300                                          \
           -flto                                                              \
           -Wl,--gc-sections                                                  \
           -Wl,--defsym,start=0x80400000                                      \
           -mabi=32                                                           \
           uss64.o sm64.o resource.o gfx.o gu.o grc.o zu.o vector.o startup.o \
           -o uss64.elf
$BIN_ROOT/${CROSS}objcopy -O binary uss64.elf uss64.bin
