cd $(dirname $(readlink -f $0))
BIN_ROOT=/opt/n64/bin/
OBJ_DIR=obj/
#BIN_ROOT=/opt/n64-dev/usr/bin
#BIN_ROOT=/usr/bin/
N64_SYSROOT=/opt/n64/mips64/n64-sysroot/usr
#N64_SYSROOT=/opt/n64-dev/mips64/n64-sysroot/usr
CFLAGS='-std=gnu11 -Wall -ffunction-sections -fdata-sections -DF3D_GBI -DSM64_U '
CFLAGS+='-O3 -flto -ffat-lto-objects'
#CFLAGS+='-Og -g'
CROSS=mips64-
$BIN_ROOT/grc                                         -d gz/res/resources.json     gz/res/gz/fipps.png                          -o ${OBJ_DIR}/fipps.png.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                           ${N64_SYSROOT}/include/grc.c                 -o ${OBJ_DIR}/grc.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                           ${N64_SYSROOT}/include/vector/vector.c       -o ${OBJ_DIR}/vector.o
$BIN_ROOT/${CROSS}gcc                         $CFLAGS -c                           ${N64_SYSROOT}/include/startup.c             -o ${OBJ_DIR}/startup.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include uss64.c                                      -o ${OBJ_DIR}/uss64.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include sm64.c                                       -o ${OBJ_DIR}/sm64.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/resource.c                         -o ${OBJ_DIR}/resource.o
$BIN_ROOT/${CROSS}gcc -Wa,-adhln -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/gfx.c                              -o ${OBJ_DIR}/gfx.o > ${OBJ_DIR}/gfx.s
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/gu.c                               -o ${OBJ_DIR}/gu.o
$BIN_ROOT/${CROSS}gcc -DZ64_VERSION=Z64_OOT10 $CFLAGS -c -I ${N64_SYSROOT}/include gz/src/gz/zu.c                               -o ${OBJ_DIR}/zu.o

cd ${OBJ_DIR}
LDFLAGS='-O3 -flto -nostartfiles -specs=nosys.specs -Wl,--defsym,start=0x80400000 -Wl,--gc-sections '
$BIN_ROOT/${CROSS}g++                                                         \
           -T ${N64_SYSROOT}/lib/gl-n64.ld ${LDFLAGS}                         \
           uss64.o sm64.o resource.o gfx.o gu.o grc.o zu.o vector.o startup.o fipps.png.o \
           -o uss64.elf
$BIN_ROOT/${CROSS}objcopy -O binary uss64.elf uss64.bin
cd ../
cp ${OBJ_DIR}/uss64.bin .