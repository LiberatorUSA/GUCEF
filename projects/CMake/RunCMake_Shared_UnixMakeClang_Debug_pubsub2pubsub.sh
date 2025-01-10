#!/bin/bash
#

# Turn on tracing, we want to see what's going on
#set -x

clear

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
RUNCMAKE_SCRIPTSTARTDIR=${scriptPath%/*}
echo "RUNCMAKE_SCRIPTSTARTDIR = $RUNCMAKE_SCRIPTSTARTDIR"

# we want to use Clang
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
clangVersion=echo clang -v
echo $clangVersion
./../check_clang_gcc.sh

echo "*** Invoking CMakeCommon ***"
. $RUNCMAKE_SCRIPTSTARTDIR/CMakeCommon.sh

# Set target specific folders
MAINCMAKE=$SRCROOTDIR/projects/CMake/targets/GUCEF_exe_pubsub2pubsub
NIXMAKE_OUTPUTDIR="$OUTPUTDIR/UnixMakeDbgClang_pubsub2pubsub"

echo "*** Invoking CMake ***"
echo "Output dir for this CMake target = $NIXMAKE_OUTPUTDIR"
echo "CMake will use source root: $MAINCMAKE"
cmake -D_CMAKE_TOOLCHAIN_PREFIX=llvm- -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=debug -G"Unix Makefiles" -H"$MAINCMAKE" -B$NIXMAKE_OUTPUTDIR
echo "Press enter to continue..."
line=""
read line
