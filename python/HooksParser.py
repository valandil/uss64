# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-06-04                                                    #
# Description:  Parses the yaml file containing the hooks, and writes the     #
#               sm64.h/c files.                                               #
# --------------------------------------------------------------------------- #

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

    # -- Longest name of variables, functions, constants, and hooks.
    string_len = []
    for key  in ['Variables', 'Functions', 'Hooks', 'Constants']:
      string_len.append(max(map(len,self.sm64_hooks[key])))

    self.max_string = max(string_len) + len("_addr")

    # -- Longest variable type.
    type_len_var = []
    for k, v in self.sm64_hooks['Variables'].items():
      type_len_var.append(len(v['VariableType']))

    self.varLen = max(type_len_var)

    # -- Longest return type.
    type_len_func = []
    for k, v in self.sm64_hooks['Functions'].items():
      type_len_func.append(len(v['ReturnType']))

    self.typeLen = max(type_len_func)

    # -- Longest function name.
    self.functionNameLen = max(map(len,self.sm64_hooks['Functions']))

    # -- Length of argument list + formatting: (arg1, arg2, arg3)
    arg_list_func = []
    for k, v in self.sm64_hooks['Functions'].items():
      arg_list_func.append(len('('+
                               ', '.join(v['Arguments'])+')' if v['Arguments'] != None else "()"))

    self.argListLen = max(arg_list_func)

  def getUSS64Hooks(self):
    return self.uss64_hooks

  def parseInputFiles(self):
    """
    Parses the hooks file to determine the #include directives to add to the
    SM64 header file.
    """
    inputFiles = ""
    if self.sm64_hooks['InputFiles']['System'] != None:
      for element in self.sm64_hooks['InputFiles']['System']:
        inputFiles += "#include <{}>\n".format(element)

      inputFiles += "\n"

    if self.sm64_hooks['InputFiles']['Local'] != None:
      for element in self.sm64_hooks['InputFiles']['Local']:
        inputFiles += "#include \"{}\"\n".format(element)

      inputFiles += "\n"

    return inputFiles

  def parseVariableDefines(self,version):
    """
    Parses the SM64 hooks file for the variables and prints the #define
    directives to be added to the SM64 header file.
    """
    varDefines = ""

    for k, v in self.sm64_hooks['Variables'].items():
      if v['Addresses'][version] != None:
        varDefines += "#define {:<{}s} 0x{:08X}\n".format(k+"_addr",
        self.max_string, v['Addresses'][version])

    if varDefines != "":
      varDefines += "\n"

    return varDefines

  def parseFunctionDefines(self,version):
    """
    Parses the SM64 hooks file for the function addresss and prints and the
    #define directives to be added to the SM64 header file.
    """
    funcDefines = ""

    for k, v in self.sm64_hooks['Functions'].items():
      if (v['Addresses'][version] != None):
        funcDefines += "#define {:<{}s} 0x{:08X}\n".format(k+"_addr", self.max_string,
                                                          v['Addresses'][version])

    if funcDefines != "":
      funcDefines += "\n"

    return funcDefines

  def parseHooks(self,version):
    """
    Parses the SM64 hooks file for the addresses at which we hook and prints
    the #define directives to be added to the SM64 header file.
    """
    hookDefines = ""

    for k, v in self.sm64_hooks['Hooks'].items():
      if v['Addresses'][version] != None:
        hookDefines += "#define {:<{}s} 0x{:08X}\n".format(k, self.max_string,
                                                        v['Addresses'][version])

    if hookDefines != "":
      hookDefines += "\n"

    return hookDefines

  def parseConstants(self):
    """
    Parses the SM64 hooks file for the value of constants. Constants do not
    vary by version.
    """
    constDefines = ""

    for k, v in self.sm64_hooks['Constants'].items():
        constDefines += "#define {:<{}s} 0x{:08X}\n".format(k, self.max_string,v)

    if constDefines != "":
      constDefines += "\n"

    return constDefines


  def parseVariableAssignments(self):
    """
    Parses the function names and their addresses to set the variables
    to their memory addresses.
    """
    varDefines = ""

    for k, v in self.sm64_hooks['Variables'].items():
      varDefines += "#define {:<{}s} (*({:<{}s}{})\n".format(
        k, self.max_string,
        v['VariableType']+"*)", self.varLen + 3,
        k+"_addr")

    if varDefines != "":
      varDefines += "\n"

    return varDefines

  def parseFunctionDeclarations(self):
    """
    Parses the function return types, names, and argument list in order to build
    the function declarations to put in the SM64 header file.
    """
    funcDefines = ""

    for k, v in self.sm64_hooks['Functions'].items():
      funcDefines += "{:<{}s} {:<{}s} ({});\n".format(
        v['ReturnType'], self.typeLen,
        "(*"+k+")", self.functionNameLen + 3,
        ', '.join(v['Arguments']) if v['Arguments'] != None else "")

    if funcDefines != "":
      funcDefines += "\n"

    return funcDefines

  def parseFunctionDefinitions(self):
    """
    Parses the function return type, name, and argument list in order to bulid
    the function definitions to put in the SM64 source file.
    """
    funcDefines = ""

    for k, v in self.sm64_hooks['Functions'].items():
      funcDefines += "{:<{}s} {:<{}s} {:<{}s} = (void*){};\n".format(
        v['ReturnType'], self.typeLen,
        "(*"+k+")", self.functionNameLen + 3,
        '('+', '.join(v['Arguments'])+')' if v['Arguments'] != None else "()",
        self.argListLen,
        k+"_addr")

    funcDefines += "\n"

    return funcDefines

  def generateSM64Header(self):
    """
    Composes the functions above to create the SM64 header file.
    """
    sm64_h = "#ifndef SM64_H\n#define SM64_H\n\n"

    sm64_h += self.parseInputFiles()

    for version in self.sm64_hooks['Versions']:
      sm64_h += "#ifdef SM64_{}\n\n".format(version)

      sm64_h += "// Variables.\n"
      sm64_h += self.parseVariableDefines(version)

      sm64_h += "// Functions.\n"
      sm64_h += self.parseFunctionDefines(version)

      sm64_h += "// Hooks.\n"
      sm64_h += self.parseHooks(version)
      sm64_h += "#endif // SM64_{}\n\n".format(version)

    sm64_h += "// Constants valid for all versions.\n"
    sm64_h += self.parseConstants()

    sm64_h += "// Variable assignment.\n"
    sm64_h += self.parseVariableAssignments()

    sm64_h += "// Function declarations.\n"
    sm64_h += self.parseFunctionDeclarations()

    sm64_h += "#endif // SM64_H\n"

    return sm64_h

  def generateSM64CFile(self):
    sm64_c = "#include \"sm64.h\"\n\n"

    sm64_c += self.parseFunctionDefinitions()

    return sm64_c
