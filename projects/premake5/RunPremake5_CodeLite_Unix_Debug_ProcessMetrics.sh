#!/bin/bash
#

# Turn on tracing, we want to see what's going on
#set -x

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
PREMAKE5LAUNCH_SCRIPTSTARTDIR=${scriptPath%/*}
echo "PREMAKE5LAUNCH_SCRIPTSTARTDIR = $PREMAKE5LAUNCH_SCRIPTSTARTDIR"

clear

echo "*** Invoking Premake5Common ***"
. Premake5Common.sh

# Set target specific folders
PM5SUBDIR="PM5_CLNIXDBG_ProcessMetrics"
PREMAKE5LAUNCH_SCRIPTSTARTDIR=${SRCROOTDIR}/projects/premake5/targets/GUCEF_exe_ProcessMetrics

echo "*** Invoking Premake ***"
PM5OUTPUTDIR="$OUTPUTDIR/$PM5SUBDIR"
echo "Output dir for this Premake5 target = $PM5OUTPUTDIR"
echo "Premake5 will use source root: $SRCROOTDIR"
echo "Premake5 will use target main premake root: $PREMAKE5LAUNCH_SCRIPTSTARTDIR"
#cd $GUCEF_HOME
#. $PREMAKE5LAUNCH_SCRIPTSTARTDIR/premake5 codelite

cd ${SRCROOTDIR}/projects/premake5
./premake5 --file=$PREMAKE5LAUNCH_SCRIPTSTARTDIR/premake5.lua codelite

echo "Press enter to continue..."
line=""
read line
