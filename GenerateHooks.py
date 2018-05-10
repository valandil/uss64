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
addrs = []

nm_cmd = "mips64-nm {}".format(args.elf)
split_nm_cmd = shlex.split(nm_cmd)
proc = subprocess.Popen(split_nm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
result, errors = proc.communicate(timeout=15)
result = result.decode('utf-8')
result = result.splitlines()

for i in range(len(hooks)):
  # Match the line with the proper function call.
  regex = re.compile(r"[0-9a-fA-F]{8}(?=.* "+"{})".format(hooks[i]))

  for j in range(len(result)):
    match = regex.search(result[j])
    if match:
      addrs.append(match.group(0))
      break


# -- Preprocess the header file with the addresses.
strings_to_replace = ["SM64_RAMEntryPoint",         \
                      "SM64_DMAHookCode",            \
                      "SM64_DMAHookJump",            \
                      "SM64_ROMPadding",             \
                      "SM64_ROMMainHook",            \
                      "SM64_CleanUpDisplayListHook" \
                      ]
sub_cmd   = 'printf \\"#include \\"sm64.h\\"\n{}\\"'.format(strings_to_replace[0])+" | mips64-gcc -E -DSM64_{} -xc -".format(args.version)
split_cmd = shlex.split(sub_cmd)
print(split_cmd)

proc = subprocess.Popen(split_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
result, errors = proc.communicate(timeout=15)
print(result.decode("utf-8"))

# -- Populate the armips script with the proper addresses.
# # -- Create the n64split-compatible YAML file and populate it with the addresses.
# addr_file = open("hooks.yaml", "w", newline=None)
# data = """
#   hooks:
#     -[_start, 0x80240000]
# """
# yaml.dump(data, addr_file, default_flow_style=None)
# print(yaml.dump(data))
# print(data)
