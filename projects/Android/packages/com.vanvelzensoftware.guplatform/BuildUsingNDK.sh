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

cd $CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR/../.. 
bash BuildUsingNDK.sh

echo " "
echo "*** Invoking binary copy script ***"
echo " "

cd $CALLGUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR 
bash CopyPackageBinaries.sh

echo "Press enter to continue..."
line=""
read line

