#!/bin/bash
#

# Turn on tracing, we want to see what's going on
#set -x

clear

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
RUNCMAKE_SCRIPTSTARTDIR=${scriptPath%/*}
echo "RUNCMAKE_SCRIPTSTARTDIR = $RUNCMAKE_SCRIPTSTARTDIR"

echo "*** Invoking CMakeCommon ***"
. $RUNCMAKE_SCRIPTSTARTDIR/CMakeCommon.sh

# Set target specific folders
MAINCMAKE=$SRCROOTDIR/projects/CMake/targets/GUCEF
KDEV3_OUTPUTDIR="$OUTPUTDIR/KDEV3"

echo "*** Invoking CMake ***"
echo "Output dir for this CMake target = $KDEV3_OUTPUTDIR"
echo "CMake will use source root: $MAINCMAKE"
cmake -DBUILD_SHARED_LIBS=ON -G"KDevelop3" -H"$MAINCMAKE" -B$KDEV3_OUTPUTDIR
echo "Press enter to continue..."
line=""
read line
