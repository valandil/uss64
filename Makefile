# --------------------------------------------------------------------------- #
# Author:        Joey Dumont         <joey.dumont@gmail.com>                  #
# Date:          2018-07-26                                                   #
# Decsription:   Compiles uss64 and generates executables capable of          #
#                inserting the payload into the original ROMs, or to          #
#                produce patches in different formats.                        #
# --------------------------------------------------------------------------- #

# Binaries
ARMIPS            := armips
CROSS             ?= mips64-ultra-elf
TOOLS_PREFIX      ?= /usr/bin/
AS                := $(CROSS)-as
CC                := $(CROSS)-gcc
CXX               := $(CROSS)-g++
LD                := $(CROSS)-g++
OBJCOPY           := $(CROSS)-objcopy
NM                := $(CROSS)-nm
GBI_VERSION       := -DF3D_GBI
PYTHON            := python3
PARSEHOOKS        := python/ParseHooks.py
GENERATEHOOKS     := python/GenerateHooks.py
GRC               := AS=$(CROSS)-as $(TOOLS_PREFIX)grc
N64CHECKSUM       := n64cksum
XDELTA            := xdelta3

# Compiler/linker flags.
N64_SYSROOT      ?= /usr/$(CROSS)/n64-sysroot/usr/
ASFLAGS           = -Wall -mtune=vr4300 -march=vr4300 -mabi=32
CFLAGS            = -std=gnu11 -Wall -O1 -mtune=vr4300 -march=vr4300 -mabi=32  \
                    -mno-check-zero-division -mdivide-breaks                   \
                    $(GBI_VERSION)                                             \
                    -DZ64_VERSION=Z64_OOT10                                    \
                    -DSETTINGS_HEADER=../../../src/settings.h                  \
                    -I ${N64_SYSROOT}/include/                                 \
                    -I $(CURDIR)                                               \
                    $(SM64_VERSION_FLAG)
CXXFLAGS          = -std=gnu++14 -Wall -O1 -mtune=vr4300 -march=vr4300 -mabi=32\
                    $(GBI_VERSION)
LDSCRIPT          = $(N64_SYSROOT)/lib/gl-n64.ld
LDFLAGS           = -T $(LDSCRIPT) -T uss64.ld -nostartfiles -specs=nosys.specs\
                    -march=vr4300 -mtune=vr4300 -mabi=32 -mdivide-breaks       \
                    -mno-check-zero-division                                   \
                    -Wl,--gc-sections                                          \
                    -Wl,--defsym,start=0x80400000

# Directories.
ASMDIR            = asm
SRCDIR            = src
RESDIR            = gz/res
RESDESC           = $(RESDIR)/resources.json
OBJDIR            = obj
BINDIR            = bin
PATCHDIR          = patch
DEBUG_SCRIPTS_OUT = debug_scripts_out
EMU_SCRIPTDIR     = c/Users/Joey/Documents/VGs/Emulation/Project64d/Scripts

# Source file lists.
ASMFILES          = $(ASMDIR)/respawn_object_hook2.s
USS64FILES        = $(SRCDIR)/uss64_commands.c $(SRCDIR)/uss64.c               \
                    $(SRCDIR)/sm64.c $(SRCDIR)/gz_api.c                        \
                    $(SRCDIR)/settings.c $(SRCDIR)/uss64_settings.c            \
                    $(SRCDIR)/uss64_warps.c $(SRCDIR)/uss64_timer.c
STDFILES          = $(N64_SYSROOT)/include/grc.c                               \
				$(N64_SYSROOT)/include/vector/vector.c                     \
				$(N64_SYSROOT)/include/startup.c                           \
				$(N64_SYSROOT)/include/list/list.c
STDHEADERS       := $(patsubst %.c, %.h, $(STDFILES))
STDHEADERS       += $(N64_SYSROOT)/include/n64.h
GZFILES           = gz/src/gz/gfx.c                                            \
                    gz/src/gz/input.c                                          \
                    gz/src/gz/resource.c                                       \
                    gz/src/gz/gu.c                                             \
                    gz/src/gz/zu.c                                             \
                    gz/src/gz/menu*.c
GZHEADERS         = $(wildcard gz/res/gz/*.h)
RESFILES          = gz/res/gz/*.png
HEADERS           = $(SRCDIR)/sm64.h

# Source files variables.
ASMSRC           := $(foreach  s, $(ASMFILES),  $(wildcard $(s)))
USS64SRC         := $(foreach  s, $(USS64FILES),$(wildcard $(s)))
STDSRC           := $(foreach  s, $(STDFILES),  $(wildcard $(s)))
GZSRC            := $(foreach  s, $(GZFILES),   $(wildcard $(s)))
RESSRC           := $(foreach  s, $(RESFILES),  $(wildcard $(s)))

OBJECTS           = $(ASM_OBJECTS) $(USS64_OBJECTS) $(STD_OBJECTS) $(GZ_OBJECTS) $(RES_OBJECTS)

# Versions of SM64 to inject into.
USS64_VERSIONS    = SM64_U SM64_J SM64_S #SM64_Q SM64_E SM64_D

# Default targets.
USS64             = $(foreach v,$(USS64_VERSIONS),uss64-$(v))
HOOKS             = $(foreach v,$(USS64_VERSIONS),GenerateHooks-uss64-$(v))
PATCHES           = $(foreach v,$(USS64_VERSIONS),patch-uss64-$(v))

# Makefile options
.SECONDEXPANSION:
.SUFFIXES:

# -- Targets
all:  $(USS64) $(HOOKS) $(PATCHES)

# -- List all possible targets.
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | xargs

# Catch-all rule to make directories.
%/:
	mkdir -p $@

.PHONY: clean list

src/sm64.h:
	$(PYTHON) $(PARSEHOOKS)

scripts: GenerateHooks
	cp $(DEBUG_SCRIPTS_OUT)/uss64* $(EMU_SCRIPTDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DEBUG_SCRIPTS_OUT)

# Purpose:
#   - for each item of USS64_VERSIONS, compile all relevant resource, C and
#     C++ files to object files;
#   - combine all object files into an ELF;
#   - convert to binary file.

# We define a template for the generation of the uss64.bin binary
# that is to be copied into the appropriate SM64 ROM.
# Arguments:
#	- $(1): Target prefixes
#   - $(2): Short name of target.
#   - $(3): Output dir for the target.
#   - $(4): Source dir for the target.
#   - $(5): Resource dir for the target.
#   - $(6): Object output dir for the target.
#   - $(7): Binary output dir for the target.
define GenerateBinary =
# -- Prepping directories.
TARGET-NAME-$(1)      = $(2)
OUTPUT-DIR-$(1)       = $(3)
SRCDIR-$(1)           = $(4)
RESDIR-$(1)           = $(5)
OBJDIR-$(1)   	       = $(6)
BINDIR-$(1)           = $(7)

VERSION-$(1)          = $$(shell echo $(3) | tail -c 2)

# -- Variables for pattern rules.
ASM_OBJECTS-$(1)      := $$(patsubst %, $$(OBJDIR-$(1))/%.o,$$(ASMSRC))
USS64_OBJECTS-$(1)    := $$(patsubst %, $$(OBJDIR-$(1))/%.o,$$(USS64SRC))
STD_OBJECTS-$(1)      := $$(patsubst %, $$(OBJDIR-$(1))/%.o,$$(STDSRC))
GZ_OBJECTS-$(1)       := $$(patsubst %, $$(OBJDIR-$(1))/%.o,$$(GZSRC))
RES_OBJECTS-$(1)      := $$(patsubst %, $$(OBJDIR-$(1))/%.o,$$(RESSRC))

# -- Short names for the main C targets.
OBJ-$(1)              = $$(ASM_OBJECTS-$(1)) $$(USS64_OBJECTS-$(1)) $$(STD_OBJECTS-$(1)) $$(GZ_OBJECTS-$(1)) $$(RES_OBJECTS-$(1))
BIN-$(1)              = $$(BINDIR-$(1))/$$(TARGET-NAME-$(1)).bin
ELF-$(1)              = $$(BINDIR-$(1))/$$(TARGET-NAME-$(1)).elf

# -- Short name of binary target to easily add to global all target.
BUILD-$(1)            = $(1)
$$(BUILD-$(1))        : $$(BIN-$(1))

$$(BIN-$(1))          : $$(ELF-$(1)) | $$$$(dir $$$$@)
	$$(OBJCOPY) -S -O binary $$< $$@

$$(ELF-$(1))          : $$(OBJ-$(1)) | $$$$(dir $$$$@)
	$$(LD) $$(LDFLAGS) -Xlinker -Map=uss64_$$(VERSION-$(1)).map $$^ -o $$@

$$(ASM_OBJECTS-$(1)): $$(OBJDIR-$(1))/%.o : % src/sm64.h | $$$$(dir $$$$@)
	$$(AS) $$(ASFLAGS) $$< -o $$@

$$(USS64_OBJECTS-$(1)): $$(OBJDIR-$(1))/%.o : % src/sm64.h | $$$$(dir $$$$@)
	$$(CC) $$(CFLAGS) -c $$< -o $$@

$$(USS64_OBJECTS-$(1)): SM64_VERSION_FLAG = -D$(3)

$$(STD_OBJECTS-$(1))  : $$(OBJDIR-$(1))/%.o : % | $$$$(dir $$$$@)
	$$(CC) $$(CFLAGS) -c $$< -o $$@

$$(STD_OBJECTS-$(1)): SM64_VERSION_FLAG = -D$(3)

$$(GZ_OBJECTS-$(1))   : $$(OBJDIR-$(1))/%.o : % | $$$$(dir $$$$@)
	$$(CC) $$(CFLAGS) -c $$< -o $$@

$$(GZ_OBJECTS-$(1)): SM64_VERSION_FLAG = -D$(3)

$$(RES_OBJECTS-$(1))  : $$(OBJDIR-$(1))/%.o  : % | $$$$(dir $$$$@)
	$$(GRC) $$< -d $$(RESDESC) -o $$@

$$(RES_OBJECTS-$(1)): SM64_VERSION_FLAG = -D$(3)

# -- Use F3D_BETA GBI for all versions except Shindou.
ifneq ($(3),SM64_S)
$$(USS64_OBJECTS-$(1)) : GBI_VERSION += -DF3D_BETA
$$(STD_OBJECTS-$(1)) : GBI_VERSION += -DF3D_BETA
$$(RES_OBJECTS-$(1)) : GBI_VERSION += -DF3D_BETA
$$(GZ_OBJECTS-$(1)) : GBI_VERSION += -DF3D_BETA
endif

GenerateHooks-$(1)    : $$(ELF-$(1)) | $$(DEBUG_SCRIPTS_OUT)/ $$(PATCHDIR)/
	$$(PYTHON) $$(GENERATEHOOKS) --elf $$(ELF-$(1)) --version $$(VERSION-$(1)) --mips64-nm $$(CROSS)-nm --sm64-hooks sm64_hooks.yml --uss64-hooks uss64_hooks.yml

.ONESHELL:
patch-$(1)            : GenerateHooks-$(1)
	cd $$(PATCHDIR)
	$$(ARMIPS) hook_$$(VERSION-$(1)).asm
	$$(N64CHECKSUM) uss64_$$(VERSION-$(1)).z64

	$$(XDELTA) -f -e -s SM64_$$(VERSION-$(1)).z64 uss64_$$(VERSION-$(1)).z64 uss64_$$(VERSION-$(1)).xdelta

.PHONY                : $$(BUILD-$(1)) GenerateHooks-$(1) patch-$(1)
endef

# -- Generate the targets for all uss64_versions.
$(foreach v, $(USS64_VERSIONS), $(eval \
	$(call GenerateBinary,uss64-$(v),uss64,$(v),$(SRCDIR)/,$(RESDIR)/$(v),$(OBJDIR)/$(v),$(BINDIR)/$(v)) \
))
