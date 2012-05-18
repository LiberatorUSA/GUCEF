#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

clear
echo "*** Invoking CMakeCommon ***"
. CMakeCommon.sh
echo "*** Invoking CMake ***"
NIXMAKE_OUTPUTDIR="$OUTPUTDIR/UnixMake"
echo "Output dir for this CMake target = $NIXMAKE_OUTPUTDIR"
echo "CMake will use source root: $SRCROOTDIR"
cmake -DBUILD_SHARED_LIBS=ON -G"Unix Makefiles" -H"$SRCROOTDIR" -B$NIXMAKE_OUTPUTDIR -DCMAKE_BUILD_TYPE:STRING=Release
echo "Press enter to continue..."
line=""
read line
