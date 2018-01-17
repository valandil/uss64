## About

Skeleton of a SM64 practice ROM. While the (provisional) title ostensibly refers to
a practice ROM to be used for speedrun practice, I don't think I have (yet) the
skills and free time necessary to provide a very polished product. Indeed, I plan
to use this project more as a way to learn more about low-level programming
(at least, lower level than I'm used to), and assembly.

## Building

To compile this C hack and patch the ROM, you will need [glank](https://github.com/glankk)'s
N64 [toolchain](https://github.com/glankk/n64). If you're on Arch Linux, simply use
```
yaourt -S mips64-gcc-n64
```
and that should install all the proper dependencies. For now, I assume that
you have installed the toolchain in `/opt/n64-dev`. Sorry about that, I'll fix
this later.

To compile the practice hack, simply run `bash compile.sh`. Then, to patch the
ROM, `cd` to the `patch` directory and run `gru make-rom.lua <your-sm64-rom-here>`.
