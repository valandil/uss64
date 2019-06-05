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
sm64_c = hooksParser.generateSM64CFile()

with open("../src/sm64.h", 'w') as sm64_h_out:
    sm64_h_out.write(sm64_h)

with open("../src/sm64.c", 'w') as sm64_c_out:
    sm64_c_out.write(sm64_c)
