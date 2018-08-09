# --------------------------------------------------------------------------- #
# Author:        Joey Dumont         <joey.dumont@gmail.com>                  #
# Date:          2018-07-26                                                   #
# Decsription:   Compiles uss64 and generates executables capable of          #
#                inserting the payload into the original ROMs, or to          #
#                produce patches in different formats.                        #
# --------------------------------------------------------------------------- #
# Binaries
ARMIPS            = armips
CROSS             = mips64-
CC                = $(CROSS)gcc
CXX               = $(CROSS)g++
LD                = $(CROSS)g++
OBJCOPY           = $(CROSS)objcopy
NM                = $(CROSS)nm
PYTHON            = python
GENERATEHOOKS     = GenerateHooks.py
GRC               = AS=$(CROSS)as grc

# Compiler/linker flags
N64_SYSROOT       = /opt/n64/mips64/n64-sysroot/usr/
CFLAGS            = -std=gnu11 -Wall -O1 -mtune=vr4300 -march=vr4300 -mabi=32  \
                    -DF3D_GBI                                                  \
                    -DZ64_VERSION=Z64_OOT10                                    \
                    -DSM64_U                                                   \
                    -I ${N64_SYSROOT}/include 
CXXFLAGS          = -std=gnu++14 -Wall -O1 -mtune=vr4300 -march=vr4300 -mabi=32\
                    -DF3D_GBI
LDFLAGS           = -T $(LDSCRIPT) -nostartfiles -specs=nosys.specs            \
                    -Wl,--gc-sections                                          \
                    -Wl,--defsym,start=0x80400000
LDSCRIPT          = gl-n64.ld

# Directories.
SRCDIR            = src
RESDIR            = gz/res
RESDESC           = $(RESDIR)/resources.json
OBJDIR            = obj
BINDIR            = bin
PATCHDIR          = patch
DEBUG_SCRIPTS_OUT = debug_scripts_out
EMU_SCRIPTDIR     = c/Users/Joey/Documents/VGs/Emulation/Project64d/Scripts

USS64FILES        = uss64.c sm64.c
STDFILES          = $(N64_SYSROOT)/include/grc.c                               \
				    $(N64_SYSROOT)/include/vector/vector.c                     \
				    $(N64_SYSROOT)/include/startup.c
STDHEADERS       := $(patsubst %.c, %.h, $(STDFILES))
STDHEADERS       += $(N64_SYSROOT)/include/n64.h
GZFILES           = gz/src/gz/gfx.c                                            \
                    gz/src/gz/resource.c                                       \
                    gz/src/gz/gu.c                                             \
                    gz/src/gz/zu.c
RESFILES          = gz/res/gz/fipps.png
HEADERS           = sm64.h

USS64SRC         := $(foreach  s, $(USS64FILES),$(wildcard $(s)))
USS64_OBJECTS    := $(patsubst %, $(OBJDIR)/%.o,$(USS64SRC))

STDSRC           := $(foreach  s, $(STDFILES),  $(wildcard $(s)))
STD_OBJECTS      := $(patsubst %, $(OBJDIR)/%.o,$(STDSRC))

GZSRC            := $(foreach  s, $(GZFILES),   $(wildcard $(s)))
GZ_OBJECTS       := $(patsubst %, $(OBJDIR)/%.o,$(GZSRC))

RESSRC           := $(foreach  s, $(RESFILES), $(wildcard $(s)))
RES_OBJECTS      := $(patsubst %, $(OBJDIR)/%.o,$(RESSRC))

OBJECTS           = $(USS64_OBJECTS) $(STD_OBJECTS) $(GZ_OBJECTS) $(RES_OBJECTS)
# Versions of SM64 to inject into.
USS64_VERSIONS    = SM64_U SM64_J

# Makefile options
.SECONDEXPANSION:
.SUFFIXES:

# Goal: 
#   - for each item of USS64_VERSIONS, compile all relevant resource, C and
#     C++ files to object files;
#   - combine all object files into an ELF;
#   - converto to binary file.

.PHONY: clean GenerateHooks patch scripts

# Default targets.
all: $(BINDIR)/uss64.bin GenerateHooks

$(USS64_OBJECTS): $(OBJDIR)/%.o : % | $$(dir $$@)
	$(CC) $(CFLAGS) -c $< -o $@

$(STD_OBJECTS): $(OBJDIR)/%.o : % $(STDHEADERS) | $$(dir $$@)
	$(CC) $(CFLAGS) -c $< -o $@

$(GZ_OBJECTS): $(OBJDIR)/%.o : % | $$(dir $$@)
	$(CC) $(CFLAGS) -c $< -o $@

$(RES_OBJECTS): $(OBJDIR)/%.o : % | $$(dir $$@)
	$(GRC) -d $(RESDESC) $^ -o $@

$(BINDIR)/uss64.elf: $(OBJECTS) | $$(dir $$@)
	$(LD) $(LDFLAGS) $^ -o $@

$(BINDIR)/uss64.bin: $(BINDIR)/uss64.elf
	$(OBJCOPY) -O binary $< $@

GenerateHooks: $(BINDIR)/uss64.elf | $(DEBUG_SCRIPTS_OUT)/ $(PATCHDIR)/
	$(PYTHON) $(GENERATEHOOKS) $(BINDIR)/uss64.elf U

# Explicit targets.
patch: GenerateHooks
	cd patch
	$(ARMIPS) hook_U.asm
	$(N64CHECKSUM) uss64_U.z64

scripts: GenerateHooks
	cp $(DEBUG_SCRIPTS_OUT)/uss64* $(EMU_SCRIPTDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DEBUG_SCRIPTS_OUT)

# Catch-all rule to make directories.
%/:
	mkdir -p $@