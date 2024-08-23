#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x


# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
PREMAKE5COMMON_SCRIPTSTARTDIR=${scriptPath%/*}
echo "PREMAKE5COMMON_SCRIPTSTARTDIR = $PREMAKE5COMMON_SCRIPTSTARTDIR"

echo "*** Perform common Premake5 environment variable setup ***"

# Set the basic environment variables we can use for GUCEF in the rest of the script
cd "$PREMAKE5COMMON_SCRIPTSTARTDIR/../.."
GUCEF_HOME=$PWD
echo "GUCEF_HOME = $GUCEF_HOME"
export SRCROOTDIR=$PWD
echo "SRCROOTDIR = $SRCROOTDIR"


# Set the output directory in which the target directories will be placed for Premake5
export OUTPUTDIR=${OUTPUTDIR:=undefined}
if [ "$OUTPUTDIR" = "undefined" ];
then
  echo "OUTPUTDIR is undefined, setting it to default"
  OUTPUTDIR="$GUCEF_HOME/common/bin"
fi
echo "OUTPUTDIR = $OUTPUTDIR"

# Set the exact output directory in which the Premake5 will output
export PM5OUTPUTDIR=${PM5OUTPUTDIR:=undefined}
if [ "$PM5OUTPUTDIR" = "undefined" ];
then
  echo "PM5OUTPUTDIR is undefined, setting it to default"
  PM5OUTPUTDIR="$GUCEF_HOME/common/bin/premake5"
fi
echo "PM5OUTPUTDIR = $PM5OUTPUTDIR"


# Go back to where we came from
cd "$PREMAKE5COMMON_SCRIPTSTARTDIR"

