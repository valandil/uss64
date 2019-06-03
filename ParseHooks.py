# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-05-04                                                    #
# Description:  Parses the yaml file containing the hooks, and writes the     #
#               sm64.h/c files.                                               #
# --------------------------------------------------------------------------- #

# -- OS stuff
import os
import sys

import yaml
import itertools

class HooksParser(object):
  """
  This class parses the sm64 and uss64 hook files.
  They generate the sm64.h header, the sm64.c unit, and perform
  string substitution in the armips input files.
  """
  def __init__(self, sm64_hooks, uss64_hooks):
    """
    Opens the hook files and determine some of their properties.
    """
    sm64_hooks_f = sm64_hooks
    uss64_hooks_f = uss64_hooks

    with open(sm64_hooks_f, 'r') as sm64_hooks_h:
      self.sm64_hooks = yaml.load(sm64_hooks_h.read())

    with open(uss64_hooks_f, 'r') as uss64_hooks_h:
      self.uss64_hooks = yaml.load(uss64_hooks_h.read())

    # -- Determine the maximum lengths of some sets of strings
    # -- in order to be able to pretty print.

    # -- Longest name of variables, functions and hooks.
    string_len = []
    for element in sm64_hooks:
      string_len.append(max(map(len,sm64_hooks[element])))

    self.max_string = max(string_len) + len("_addr")

    # -- Longest variable type.
    type_len_var = []
    for k, v in sm64_hooks['Variables'].items():
      type_len_var.append(len(v['VariableType']))

    self.varLen = max(type_len_var)

    # -- Longest return type.
    type_len_func = []
    for k, v in sm64_hooks['Functions'].items():
      type_len_func.append(len(v['ReturnType']))

    self.typeLen = max(type_len_func)

    # -- Longest function name.
    self.functionNameLen = max(map(len,sm64_hooks['Functions']))

    # -- Length of argument list + formatting: (arg1, arg2, arg3)
    arg_list_func = []
    for k, v in sm64_hooks['Functions'].items():
      arg_list_func.append(len('('+
                               ', '.join(v['Arguments'])+')' if v['Arguments'] != None else "()"))

    self.argListLen = max(arg_list_func)

  def parseInputFiles(self):
    """
    Parses the hooks file to determine the #include directives to add to the
    SM64 header file.
    """
    inputFiles = ""
    for element in sm64_hooks['InputFiles']['System']:
      inputFiles += "#include <{}>".format(element)

    if sm64_hooks['InputFiles']['System'] != None:
      inputFiles += "\n"

    for element in sm64_hooks['InputFiles']['Local']:
      inputFiles += "#include \"{}\"".format(element)

    if sm64_hooks['InputFiles']['Local'] != None:
      inputFiles += "\n"

    return inputFiles

  def parseVariableDefines(self,version):
    varDefines = ""

    if v['Addresses'][version] != None:
      for k, v in sm64_hooks['Variables'].items():
        varDefines += "#define {:<{}s} 0x{:08X}\n".format(k+"_addr",
          self.max_string,
          v['Addresses'][version])

      varDefines += "\n"

    return varDefines

# ---------------------------- Parsing Functions ---------------------------- #

def parseFunctionDefines(hooks, version):
  """
  Parses the hooks yaml file for the addresses of functions.
  """
  funcDefines = ""

  for k, v in hooks['Functions'].items():
    if (v['Addresses'][version] != None):
      funcDefines += "#define {:<{}s} 0x{:08X}\n".format(k+"_addr", max_string,
                                                        v['Addresses'][version])

  funcDefines += "\n"

  return funcDefines


def parseHooks(hooks,version):
  """
  """
  funcDefines = ""

  for k, v in hooks['Hooks'].items():
    if (v['Addresses'][version] != None):
      funcDefines += "#define {:<{}s} 0x{:08X}\n".format(k, max_string,
                                                        v['Addresses'][version])

  funcDefines += "\n"

  return funcDefines

def parseConstants(hooks):
  """
  """
  funcDefines = ""

  for k, v in hooks['Constants'].items():
      funcDefines += "#define {:<{}s} 0x{:08X}\n".format(k, max_string,v)

  funcDefines += "\n"

  return funcDefines

def parseVariableAssignments(hooks):
  """
  """
  varDefines = ""

  for k, v in hooks['Variables'].items():
    varDefines += "#define {:<{}s} (*({:<{}s}{})\n".format(k, max_string,
                                       v['VariableType']+"*)", varLen + 3, k+"_addr")

  varDefines += "\n"

  return varDefines

def parseFunctionDeclarations(hooks):
  """
  """
  varDefines = ""

  for k, v in hooks['Functions'].items():
    varDefines += "{:<{}s} {:<{}s} ({});\n".format(v['ReturnType'], typeLen, "(*"+k+")", functionNameLen+3,
                                       ', '.join(v['Arguments']) if v['Arguments'] != None else "")

  varDefines += "\n"

  return varDefines

def parseFunctionDefinitions(hooks):
  """
  """
  varDefines = ""

  for k, v in hooks['Functions'].items():
    varDefines += "{:<{}s} {:<{}s} {:<{}s} = (void*){};\n".format(v['ReturnType'], typeLen, "(*"+k+")", functionNameLen+3,
                                       '('+', '.join(v['Arguments'])+')' if v['Arguments'] != None else "()", argListLen, k+"_addr")

  varDefines += "\n"

  return varDefines

# ------------------------------ MAIN FUNCTION ------------------------------ #
# -- Change directory to where the script is located.
os.chdir(sys.path[0])

# -- Open the hooks file.
with open('hooks.yml', 'r') as hooks_file:
  hooks_yaml = yaml.load(hooks_file.read())

# -- We print the variable addresses for each version.
sm64_h = """#ifndef SM64_H
#define SM64_H

"""

sm64_h += parseInputFiles(hooks_yaml)

for version in hooks_yaml['Versions']:
  sm64_h += "#ifdef SM64_{}\n\n".format(version)

  sm64_h += "// Variables.\n"
  sm64_h += parseVariableDefines(hooks_yaml,version)

  sm64_h += "// Functions.\n"
  sm64_h += parseFunctionDefines(hooks_yaml,version)

  sm64_h += "// Hooks.\n"
  sm64_h += parseHooks(hooks_yaml,version)
  sm64_h += "#endif // SM64_{}\n\n".format(version)

sm64_h += "// Constants valid for all versions.\n"
sm64_h += parseConstants(hooks_yaml)

sm64_h += "// Variable assignment.\n"
sm64_h += parseVariableAssignments(hooks_yaml)

sm64_h += "// Function declarations.\n"
sm64_h += parseFunctionDeclarations(hooks_yaml)

sm64_h += "#endif // SM64_H\n"

sm64_c = """#include \"sm64.h\"

"""
sm64_c += parseFunctionDefinitions(hooks_yaml)

# -- Write files.
with open("src/sm64.h", 'w') as sm64_h_out:
  sm64_h_out.write(sm64_h)

with open("src/sm64.c", 'w') as sm64_c_out:
  sm64_c_out.write(sm64_c)

