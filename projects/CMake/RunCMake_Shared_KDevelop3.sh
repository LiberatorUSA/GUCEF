#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

clear
echo "*** Invoking CMakeCommon ***"
. CMakeCommon.sh
echo "*** Invoking CMake ***"
KDEV3_OUTPUTDIR="$OUTPUTDIR/KDEV3"
echo "Output dir for this CMake target = $KDEV3_OUTPUTDIR"
echo "CMake will use source root: $SRCROOTDIR"
cmake -DBUILD_SHARED_LIBS=ON -G"KDevelop3" -H"$SRCROOTDIR" -B$KDEV3_OUTPUTDIR
echo "Press enter to continue..."
line=""
read line
