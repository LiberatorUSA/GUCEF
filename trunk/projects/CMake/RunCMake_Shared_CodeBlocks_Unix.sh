#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

clear
echo "*** Invoking CMakeCommon ***"
. CMakeCommon.sh
echo "*** Invoking CMake ***"
CBNIX_OUTPUTDIR="$OUTPUTDIR/CBNIX"
echo "Output dir for this CMake target = $CBNIX_OUTPUTDIR"
echo "CMake will use source root: $SRCROOTDIR"
cmake -DBUILD_SHARED_LIBS=ON -G"CodeBlocks - Unix Makefiles" -H"$SRCROOTDIR" -B$CBNIX_OUTPUTDIR
echo "Press enter to continue..."
line=""
read line
