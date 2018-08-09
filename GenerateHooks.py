# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-05-04                                                    #
# Description:  Prepare the armips file necessary to inject uss64 into the    #
#               specified version of the ROM. Store the addresses of specified#
#               functions within the uss64 ELF in a YAML file for use with    #
#               n64split.                                                     #
# --------------------------------------------------------------------------- #

"""
TO FIX
 - Shell commands assume that the mips64-* binaries are in the path, and are
 - called mips64-{} and not mips64-elf-{}. Use the environment to generalize?
TO DO:
 - Add the sm64.*.yml config files to the repo, and add the relevant uss64
   ranges and labels for easy disassembly.
"""
import argparse
import yaml

# -- OS stuff.
import os
import sys

# -- Run shell commands.
import subprocess
import shlex

# -- Regular expressions.
import re

# ------------------------------ MAIN FUNCTION ------------------------------ #
# -- Change directory to where the scrit is located.
os.chdir(sys.path[0])

# -- Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("elf",
                  type=str,
                  help="ELF file to analyze.")
parser.add_argument("version",
                  type=str,
                  help="Version of the game to hook into (J or U).")
args = parser.parse_args()

# -- Names of the functions which we want to hook into SM64.
hooks     = ["_start",      "display_hook",      "gfx_flush",       "uss64_ready", "gfx_disp",       "gfx_disp_w"]
HookNames = ["USS64_Start", "USS64_DisplayAddr", "USS64_gfx_flush", "USS64_Ready", "USS64_gfx_disp", "USS64_gfx_disp_w"]
addrs = []

# -- Use nm to output the symbol table of uss64.
nm_cmd = "mips64-nm {}".format(args.elf)
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

# -- Determine the SM64 addresses that we hook on by
# -- preprocessing the sm64.h header by specifing the proper
# -- macro definition for the SM64 version we are currently using.
strings_to_replace = ["SM64_RAMEntryPoint",         \
                      "SM64_DMAHookCode",           \
                      "SM64_DMAHookJump",           \
                      "SM64_ROMPaddingStart",       \
                      "SM64_ROMMainHook",           \
                      "SM64_CleanUpDisplayListHook" \
                      ]
addrs_to_replace = []

# -- For each hook, determine the address that the preprocessor outputs, and
# -- write it in addrs_to_replace.
for i in range(len(strings_to_replace)):
  sub_cmd = "printf \"#include \\\"src/sm64.h\\\"\\n{}\"".format(strings_to_replace[i])+" | mips64-gcc -E -DSM64_{} -xc - | tail -n 1".format(args.version)
  addrs_to_replace.append(subprocess.check_output(sub_cmd, shell=True).strip().decode("utf-8"))

# -- Concatenate the uss64 addresses with the SM64 addresses and zip them
# -- up in a dict for easy access.
names = HookNames+strings_to_replace
addrs = addrs+addrs_to_replace
HooksDict = dict(zip(names,addrs))

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

# # -- Create the n64split-compatible YAML file and populate it with the addresses.
# addr_file = open("hooks.yaml", "w", newline=None)
# data = """
#   hooks:
#     -[_start, 0x80240000]
# """
# yaml.dump(data, addr_file, default_flow_style=None)
# print(yaml.dump(data))
# print(data)
