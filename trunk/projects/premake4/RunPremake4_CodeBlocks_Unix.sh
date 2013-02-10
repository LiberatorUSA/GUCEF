#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

clear
echo "*** Invoking Premake4Common ***"
. Premake4Common.sh
echo "*** Invoking CMake ***"
CBNIX_OUTPUTDIR="$OUTPUTDIR/PM4_CBNIX"
echo "Output dir for this CMake target = $CBNIX_OUTPUTDIR"
echo "CMake will use source root: $SRCROOTDIR"
premake4 codeblocks
echo "Press enter to continue..."
line=""
read line
