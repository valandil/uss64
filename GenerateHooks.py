# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-05-04                                                    #
# Description:  Store the addresses of specified functions within the uss64   #
#               ELF in a YAML file. Prepare the armips file necessary to      #
#               inject uss64 into the specified version of the ROM.           #
# --------------------------------------------------------------------------- #

import argparse
import yaml

# -- OS stuff.
import os
import subprocess
import shlex
import re

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
hooks = ["_start", "display_hook"]
HookNames = ["USS64_Start", "USS64_DisplayAddr"]
addrs = []

nm_cmd = "mips64-nm {}".format(args.elf)
split_nm_cmd = shlex.split(nm_cmd)
proc = subprocess.Popen(split_nm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
result, errors = proc.communicate(timeout=15)
result = result.decode('utf-8')
result = result.splitlines()

for i in range(len(hooks)):
  # Match the line with the proper function call.
  regex = re.compile(r"8[0-9a-fA-F]{7}(?=.* "+"{})".format(hooks[i]))

  for j in range(len(result)):
    match = regex.search(result[j])
    if match:
      addrs.append("0x"+match.group(0))
      break


# -- Preprocess the header file with the addresses.
strings_to_replace = ["SM64_RAMEntryPoint",         \
                      "SM64_DMAHookCode",           \
                      "SM64_DMAHookJump",           \
                      "SM64_ROMPaddingStart",       \
                      "SM64_ROMMainHook",           \
                      "SM64_CleanUpDisplayListHook" \
                      ]
addrs_to_replace = []

for i in range(len(strings_to_replace)):
  sub_cmd = "printf \"#include \\\"sm64.h\\\"\\n{}\"".format(strings_to_replace[i])+" | mips64-gcc -E -DSM64_{} -xc - | tail -n 1".format(args.version)
  addrs_to_replace.append(subprocess.check_output(sub_cmd, shell=True).strip().decode("utf-8"))

names = HookNames+strings_to_replace
addrs = addrs+addrs_to_replace

HooksDict = dict(zip(names,addrs))
print(HooksDict)


# -- Populate the armips script with the proper addresses.
with open("patch/hook.asm", 'r') as armips_script:
  armips_lines = armips_script.readlines()

with open("patch/hook_{}.asm".format(args.version), 'w') as armips_o:
  for armips_line in armips_lines:
    for key, value in HooksDict.items():
      armips_line = re.sub("{{{}}}".format(key), value, armips_line)
    armips_o.write(armips_line)
  armips_o.write("\n")
  

# -- Read the armips script.
# # -- Create the n64split-compatible YAML file and populate it with the addresses.
# addr_file = open("hooks.yaml", "w", newline=None)
# data = """
#   hooks:
#     -[_start, 0x80240000]
# """
# yaml.dump(data, addr_file, default_flow_style=None)
# print(yaml.dump(data))
# print(data)
