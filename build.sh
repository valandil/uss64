# --------------------------------------------------------------------------- #
# Author:       Joey Dumont                   <joey.dumont@gmail.com>         #
# Date:         2018-02-14                                                    #
# Description:  Simple build script.                                          #
# License:      CC0 - Public Domain                                           #
# ----------------------------------------------------------------------------#

# Check the number of arguments.
if [ $# -gt 2 ]; then
  printf "Usage is\n"
  printf "\tbash build.sh release/debug [toolchain-file]\n"
  exit 1
fi

# Change to file directory.
cd "$(dirname "$(readlink -f "$0")")";

# Check if build/ dir exists.
if [ ! -d build ]; then
    mkdir build
else
    rm -rf build
    mkdir  build
fi

# Change to build dir and compile the library.
cd build
CMAKE_FLAGS=
CMAKE_TOOLCHAIN_FILE=
if [ $# -gt 0 ]; then
  if [ $1 == release ]; then
    CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_BUILD_TYPE=Release"
  elif [ $1 == debug ]; then
    CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_BUILD_TYPE=Debug"
  fi
fi

if [ $# -gt 1 ]; then
  CMAKE_TOOLCHAIN_FILE=$1
else
  CMAKE_TOOLCHAIN_FILE=../Toolchain-mips64-linux-x86_64.cmake
fi

cmake ${CMAKE_FLAGS} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} ..
