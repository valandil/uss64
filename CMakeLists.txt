# --------------------------------------------------------------------------- #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-01-15                                                    #
# Description:  CMake compilation instructions for the SM64 practice ROM.     #
# License:      CC0 - Public Domain                                           #
# ----------------------------------------------------------------------------#

# ----------------------------------------------------------------- #
# --                 Name and version of library                 -- #
# ----------------------------------------------------------------- #
set(uss64)
set (USSR64_VERSION_MAJOR   0)
set (USSR64_VERSION_MINOR   0)
set (USSR64_VERSION_RELEASE 0)

# ----------------------------------------------------------------- #
# --               Configuration and Dependencies                -- #
# ----------------------------------------------------------------- #
# -- Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# -- CMake version and installation directory.
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX /opt/n64)
endif()

# -- Include CMake modules of this package.
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# ----------------------------------------------------------------- #
# --                    Compiler Configuration                   -- #
# ----------------------------------------------------------------- #
set (CMAKE_C_FLAGS_RELEASE   "-G 0 -O2 -Wall -mtune=vr4300 -march=vr4300 -mabi=32")
set (CMAKE_CXX_FLAGS_RELEASE "-G 0 -O2 -Wall -mtune=vr4300 -march=vr4300 -mabi=32")

add_definitions(-DZ64_VERSION=Z64_OOT10)

# ----------------------------------------------------------------- #
# --                  Compilation Instructions                   -- #
# ----------------------------------------------------------------- #
# -- Included files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(/opt/n64-dev/n64-sysroot/include)

# -- Create object.
add_library(uss64 OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/hello_world.c)

# ----------------------------------------------------------------- #
# --                  Installation Instructions                  -- #
# ----------------------------------------------------------------- #

# -- Output binaries in directory
set (CMAKE_OBJECTS_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR})
install (TARGETS uss64 OBJECTS DESTINATION bin)