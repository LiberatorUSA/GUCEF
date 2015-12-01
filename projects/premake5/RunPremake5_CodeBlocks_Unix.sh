#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
PREMAKE5LAUNCH_SCRIPTSTARTDIR=${scriptPath%/*}
echo "PREMAKE5LAUNCH_SCRIPTSTARTDIR = $PREMAKE5LAUNCH_SCRIPTSTARTDIR"

clear

echo "*** Setting premake5 output sub-dir ***"
PM5SUBDIR="PM5_CBNIX"

echo "*** Invoking Premake5Common ***"
. Premake5Common.sh

echo "*** Invoking CMake ***"
PM5OUTPUTDIR="$OUTPUTDIR/PM5_CBNIX"
echo "Output dir for this Premake5 target = $PM5OUTPUTDIR"
echo "Premake5 will use source root: $SRCROOTDIR"
cd $GUCEF_HOME
. $PREMAKE5LAUNCH_SCRIPTSTARTDIR/premake5 codeblocks
echo "Press enter to continue..."
line=""
read line
