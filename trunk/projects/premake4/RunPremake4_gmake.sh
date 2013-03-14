#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
PREMAKE4LAUNCH_SCRIPTSTARTDIR=${scriptPath%/*}
echo "PREMAKE4LAUNCH_SCRIPTSTARTDIR = $PREMAKE4LAUNCH_SCRIPTSTARTDIR"

clear

echo "*** Setting premake4 output sub-dir ***"
PM4SUBDIR="PM4_GMAKE"

echo "*** Invoking Premake4Common ***"
. Premake4Common.sh

echo "*** Invoking CMake ***"
PM4OUTPUTDIR="$OUTPUTDIR/PM4_GMAKE"
echo "Output dir for this Premake4 target = $PM4OUTPUTDIR"
echo "Premake4 will use source root: $SRCROOTDIR"
cd $GUCEF_HOME
$PREMAKE4LAUNCH_SCRIPTSTARTDIR/premake4 gmake
echo "Press enter to continue..."
line=""
read line
