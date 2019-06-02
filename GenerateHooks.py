# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-05-04                                                    #
# Description:  Prepare the armips file necessary to inject uss64 into the    #
#               specified version of the ROM.                                 #
# --------------------------------------------------------------------------- #

# -- OS stuff.
import os
import sys

# -- Argument parsing
import argparse

# -- Run shell commands.
import subprocess
import shlex

# -- Regular expressions.
import re

# ------------------------------ MAIN FUNCTION ------------------------------ #
# -- Change directory to where the script is located.
os.chdir(sys.path[0])

# -- Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("--mips64-nm",
                    type=str,
                    default="mips64-nm",
                    help="Path to mips64-nm.")
parser.add_argument("--mips64-gcc",
                    type=str,
                    default="mips64-gcc",
                    help="Path to mips64-gcc.")
parser.add_argument("--verbose", "-v",
                    action="count",
                    help="Increase the verbosity of the script.")
parser.add_argument("elf",
                    type=str,
                    help="ELF file to analyze.")
parser.add_argument("version",
                    type=str,
                    help="Version of the game to hook intos.")
args = parser.parse_args()

if (args.verbose == 1):
  print(args.version)

# -- Names of the functions which we want to hook into SM64.
hooks     = ["_start",      "display_hook",      "interaction_star_hook1",       "interaction_star_hook2",       "gfx_flush",       "uss64",       "gfx_disp",       "gfx_disp_w"]
HookNames = ["USS64_Start", "USS64_DisplayAddr", "USS64_interaction_star_hook1", "USS64_interaction_star_hook2", "USS64_gfx_flush", "USS64_Ready", "USS64_gfx_disp", "USS64_gfx_disp_w"]
addrs = []

# -- Use nm to output the symbol table of uss64.
nm_cmd = "{} {}".format(args.mips64_nm,args.elf)
split_nm_cmd = shlex.split(nm_cmd)
proc = subprocess.Popen(split_nm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
result, errors = proc.communicate(timeout=15)
result = result.decode('utf-8')
result = result.splitlines()

# -- For each element in hooks, isolate its 32-bit address, i.e. the
# -- one that stars with 8. Some toolchains will pad 32-bit addresses
# -- with 8 F's before the actual address, although I don't know why.
for i in range(len(hooks)):

  # Compile the regex that does this.
  regex = re.compile(r"8[0-9a-fA-F]{7}(?=.* "+"{})".format(hooks[i]))

  # Search the output of nm for our hook. Append the result
  # to addrs in the format 0xyyyyyyyy.
  for j in range(len(result)):
    match = regex.search(result[j])
    if match:
      addrs.append("0x"+match.group(0))
      break

if (args.verbose == 2):
  print(dict(zip(hooks,addrs)))
# -- Determine the SM64 addresses that we hook on by
# -- preprocessing the sm64.h header by specifing the proper
# -- macro definition for the SM64 version we are currently using.
strings_to_replace = ["SM64_RAMEntryPoint",          \
                      "SM64_DMAHookCode",            \
                      "SM64_DMAHookJump",            \
                      "SM64_ROMPaddingStart",        \
                      "SM64_ROMMainHook",            \
                      "SM64_CleanUpDisplayListHook", \
                      "SM64_DMACopy",                \
                      "SM64_SoundInitHook",          \
                      "osInvalDCache_addr",          \
                      "SM64_interaction_star_hook1", \
                      "SM64_interaction_star_hook2"  \
                      ]
addrs_to_replace = []

# -- For each hook, determine the address that the preprocessor outputs, and
# -- write it in addrs_to_replace.
for i in range(len(strings_to_replace)):
  sub_cmd = "printf \"#include \\\"src/sm64.h\\\"\\n{}\"".format(strings_to_replace[i])+" | {} -E -DSM64_{} -I/opt/n64/mips64/n64-sysroot/usr/include/ -xc - | tail -n 1".format(args.mips64_gcc,args.version)
  addrs_to_replace.append(subprocess.check_output(sub_cmd, shell=True).strip().decode("utf-8"))

if (args.verbose ==2):
  print(dict(zip(strings_to_replace,addrs_to_replace)))

# -- Version strings, and file paths.
FileNames = ["USS64_BIN",    \
             "SM64_ROM",     \
             "USS64_ROM",    \
             "SM64_VERSION"
             ]

# -- Determine the full path to uss64.bin.
sub_cmd = "readlink -f {}".format(args.elf)
bin_name = os.path.splitext(subprocess.check_output(sub_cmd, shell=True).strip().decode("utf-8"))[0]+".bin"

# -- Determine if we're on mingw python.
if (sys.platform == "msys"):
  bin_name = r"c:\\msys64\\"+bin_name

# -- Final names of the output files.
FilesToReplace = []
FilesToReplace.append(bin_name)
FilesToReplace += ["SM64_{}".format(args.version), \
                  "uss64_{}".format(args.version), \
                  "SM64_{}".format(args.version), \
                 ]

# -- Concatenate the uss64 addresses with the SM64 addresses and zip them
# -- up in a dict for easy access.
names = HookNames+strings_to_replace+FileNames
addrs = addrs+addrs_to_replace+FilesToReplace
HooksDict = dict(zip(names,addrs))

if (args.verbose == 2):
  print(HooksDict)

# -- Populate the armips script with the proper addresses.
with open("asm/hook.asm", 'r') as armips_script:
  armips_lines = armips_script.readlines()

with open("patch/hook_{}.asm".format(args.version), 'w') as armips_o:
  for armips_line in armips_lines:
    for key, value in HooksDict.items():
      armips_line = re.sub("{{{}}}".format(key), value, armips_line)
    armips_o.write(armips_line)
  armips_o.write("\n")

# -- For each file in scripts/, run the substitution algo.
for filename in os.listdir("debug_scripts"):
  with open("debug_scripts/{}".format(filename), 'r') as filename_input:
    filename_lines = filename_input.readlines()

  output_filename = os.path.splitext(filename)[0]
  with open("debug_scripts_out/{}_{}.js".format(output_filename,args.version), 'w') as filename_o:
    for filename_line in filename_lines:
      for key, value in HooksDict.items():
        filename_line = re.sub("{{{}}}".format(key), value, filename_line)
      filename_o.write(filename_line)
    filename_o.write("\n")
