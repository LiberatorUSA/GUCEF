#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

#------------------------------------------------------------------------------

echo " "
echo "*** Setting up basic variables ***"
echo " "

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR=${scriptPath%/*}
echo "CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR = $CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR"

echo " "
echo "*** Invoking build script ***"
echo " "

. $CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR/../../BuildUsingNDK.sh

echo " "
echo "*** Invoking binary copy script ***"
echo " "

. $CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR/CopyPackageBinaries.sh
