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

import copy
import HooksParser

# ------------------------------ MAIN FUNCTION ------------------------------ #
# -- Change directory to where the script is located.
callPath = os.getcwd()
os.chdir(sys.path[0])

# ----------------------------- Argument Parsing ---------------------------- #
# -- Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("--verbose", "-v",
                    action="count",
                    help="Increase the verbosity of the script.",
                    default=0)
parser.add_argument("--mips64-nm",
                    type=str,
                    default="mips64-nm",
                    help="Path to mips64-nm.")
parser.add_argument("--elf",
                    type=str,
                    help="ELF file to analyze.")
parser.add_argument("--sm64-hooks",
                    type=str,
                    help="Path to file containing the addresses of hooks, functions and variables in SM64.")
parser.add_argument("--uss64-hooks",
                    type=str,
                    help="Path to file containing the names of the uss64 functions and variables for which we need the address.")
parser.add_argument("--version",
                    type=str,
                    help="Version of the game to hook into.")
args = parser.parse_args()

# -- Cleanup arguments
args.sm64_hooks = os.path.join(callPath, args.sm64_hooks)
args.uss64_hooks = os.path.join(callPath, args.uss64_hooks)
args.elf = os.path.join(callPath, args.elf)
if (args.verbose == 1):
  print(args.version)

# ---------------------------- Parse Hooks Files ---------------------------- #

hooksParser = HooksParser.HooksParser(args.sm64_hooks,
                                      args.uss64_hooks)

# -- Parse the addresses of the hooks.
uss64_hooks = hooksParser.getUSS64Hooks()
uss64_addrs = copy.deepcopy(uss64_hooks)

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
for k in uss64_addrs.keys():

  # Compile the regex that does this.
  regex = re.compile(r"8[0-9a-fA-F]{7}(?=.* "+"{})".format(uss64_addrs[k]))

  # Search the output of nm for our hook. Append the result
  # to addrs in the format 0xyyyyyyyy.
  for j in range(len(result)):
    match = regex.search(result[j])
    if match:
      uss64_addrs[k] = "0x"+match.group(0)
      break

if (args.verbose > 1):
  print(uss64_addrs)

# -- Define paths and filenames that need to be replaced.
# -- Determine the full path to uss64.bin.
sub_cmd = "readlink -f {}".format(args.elf)
bin_name = os.path.splitext(subprocess.check_output(sub_cmd, shell=True).strip().decode("utf-8"))[0]+".bin"

# -- Determine if we're on mingw python.
if (sys.platform == "msys"):
  bin_name = r"c:\\msys64\\"+bin_name

PathFileNameDict = {"USS64_BIN": bin_name,
                    "SM64_ROM": "SM64_{}".format(args.version),
                    "USS64_ROM": "uss64_{}".format(args.version),
                    "SM64_VERSION": "SM64_{}".format(args.version)
                    }

# -- Generate the list of strings that we parse for and replace with
# -- addresses in the armips input files.
sm64_hooks = hooksParser.getSM64Hooks()

# -- Bring everything together.
HooksDict = {**hooksParser.getAllAddresses(args.version),
             **uss64_addrs,
             **PathFileNameDict}

if (args.verbose == 2):
  print(HooksDict)

# -- Populate the armips script with the proper addresses.
with open("../asm/hook.asm", 'r') as armips_script:
  armips_lines = armips_script.readlines()

with open("../patch/hook_{}.asm".format(args.version), 'w') as armips_o:
  for armips_line in armips_lines:
    for key, value in HooksDict.items():
      armips_line = re.sub("{{{}}}".format(key), value, armips_line)
    armips_o.write(armips_line)
  armips_o.write("\n")

# -- For each file in scripts/, run the substitution algo.
for filename in os.listdir("../debug_scripts"):
  with open("../debug_scripts/{}".format(filename), 'r') as filename_input:
    filename_lines = filename_input.readlines()

  output_filename = os.path.splitext(filename)[0]
  with open("../debug_scripts_out/{}_{}.js".format(output_filename,args.version), 'w') as filename_o:
    for filename_line in filename_lines:
      for key, value in HooksDict.items():
        filename_line = re.sub("{{{}}}".format(key), value, filename_line)
      filename_o.write(filename_line)
    filename_o.write("\n")
