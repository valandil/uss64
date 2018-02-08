cd $(dirname $(readlink -f $0))
mips64-gcc -G 0 -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -c -I /opt/n64-dev/n64-sysroot/include hello_world.c -o hello_world.o
