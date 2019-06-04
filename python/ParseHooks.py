# --------------------------------------------------------------------------  #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-05-04                                                    #
# Description:  Parses the yaml file containing the hooks, and writes the     #
#               sm64.h/c files.                                               #
# --------------------------------------------------------------------------- #

# -- OS stuff
import os
import sys

# -- Local imports
import HooksParser

# ---------------------------- Parsing Functions ---------------------------- #


# ------------------------------ MAIN FUNCTION ------------------------------ #
# -- Change directory to where the script is located.
os.chdir(sys.path[0])

hooksParser = HooksParser.HooksParser('../sm64_hooks.yml', '../uss64_hooks.yml')

sm64_h = hooksParser.generateSM64Header()

print(sm64_h)
