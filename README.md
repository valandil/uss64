## About

Skeleton of a SM64 practice ROM. While the (provisional) title ostensibly refers
to a practice ROM to be used for speedrun practice, I don't think I have (yet)
the skills and free time necessary to provide a very polished product. Indeed, I
plan to use this project more as a way to learn more about low-level programming
(at least, lower level than I'm used to), and assembly.


## Patching

To start playing this hack, you need to download a patch from the Releases
section and apply it. Use [xdelta3][1] to apply the patch
on your own ROM. It is available on most Linux software repositories.

## Building

### Dependencies

If you want to build this project from source, you will need:
  * a mips64 cross-compiler (see below for details);
  * glank's [n64 library][2] and related tools
  * python >= 3
  * [armips][3]
  * [n64cksum][4]
  * xdelta3

### mips64 toolchain

If you are building this on Windows under MSYS2, the easiest and preferred way
to install the toolchain is to follow the instructions that glank provides
in his [n64 repo][5]. This will install both the toolchain and the custom N64
development libraries.

If you are on Linux, you can either use glank's instructions, or use your own
mips64 toolchain. On Arch Linux, you can install the toolchain from the AUR by
first running

```bash
yay install mips64-elf-binutils mips64-elf-gcc-stage1 mips64-elf-newlib
```

then

```
yay install mips64-elf-gcc n64-git
```

You can also install the `mips64-ultra-elf` toolchain by sustituting
`mips64-elf` to `mips64-ultra-elf` in the commands above. This is a custom
toolchain developed by glank to easily target both the N64 and the Wii Virtual
Console.

### Running make

To successfully build, the n64 library development headers and tools must be
made available to the Makefile. There are several variables that might need to
be tuned for your system. The default configuration is to use the
`mips64-ultra-elf` toolchain with the AUR packaging standards. However, if you
followed glank's instructions, you will want to use something like:

```bash
TOOLS_PREFIX=/opt/n64/bin/ N64_SYSROOT=/opt/n64/mips64-ultra-elf/n64-sysroot/usr make
```

Make sure that the cross-compiler and the other tools mentioned in the
Dependencies section above  are in your system PATH.

## Thanks

Thanks to [@kdjmonaghan](https://github.com/kdjmonaghan) for getting me started,
[@queueRAM](https://github.com/queueRAM) and for the general help and their
immensely useful tools, and of course [@glankk](https://github.com/glankk) for
making gz in the first place.

[1]: http://xdelta.org/
[2]: https://github.com/glankk/n64
[3]: https://github.com/Kingcom/armips
[4]: https://github.com/queueRAM/sm64tools
[5]: https://github.com/glankk/n64