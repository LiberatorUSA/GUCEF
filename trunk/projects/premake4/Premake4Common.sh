#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x


# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
PREMAKE4COMMON_SCRIPTSTARTDIR=${scriptPath%/*}
echo "PREMAKE4COMMON_SCRIPTSTARTDIR = $PREMAKE4COMMON_SCRIPTSTARTDIR"

echo "*** Perform common Premake4 environment variable setup ***"

# Set the basic environment variables we can use for GUCEF in the rest of the script
cd "$PREMAKE4COMMON_SCRIPTSTARTDIR/../.."
GUCEF_HOME=$PWD
echo "GUCEF_HOME = $GUCEF_HOME"
SRCROOTDIR=$PWD
echo "SRCROOTDIR = $SRCROOTDIR"


# Set the output directory in which the target directories will be placed for Premake4
export OUTPUTDIR=${OUTPUTDIR:=undefined}
if [ "$OUTPUTDIR" = "undefined" ];
then
  echo "OUTPUTDIR is undefined, setting it to default"
  OUTPUTDIR="$GUCEF_HOME/common/bin"
fi
echo "OUTPUTDIR = $OUTPUTDIR"

# Set the exact output directory in which the Premake4 will output
export PM4OUTPUTDIR=${PM4OUTPUTDIR:=undefined}
if [ "$PM4OUTPUTDIR" = "undefined" ];
then
  echo "PM4OUTPUTDIR is undefined, setting it to default"
  PM4OUTPUTDIR="$GUCEF_HOME/common/bin/premake4"
fi
echo "PM4OUTPUTDIR = $PM4OUTPUTDIR"

# Set environment variable which points to OIS library home
OIS_HOME=${OIS_HOME:=undefined}
if [ "$OIS_HOME" = "undefined" ]; then
  echo "OIS environment variable not found, setting it"
  OIS_HOME="$GUCEF_HOME/dependencies/OIS"
fi
echo "OIS_HOME = $OIS_HOME"


# Set environment variable which points to FreeImage library home
FREEIMAGE_HOME=${FREEIMAGE_HOME:=undefined}
if [ "$FREEIMAGE_HOME" = "undefined" ]; then
  echo "FreeImage environment variable not found, setting it"
  FREEIMAGE_HOME="$GUCEF_HOME/dependencies/FreeImage"
fi
echo "FREEIMAGE_HOME = $FREEIMAGE_HOME"


# Set environment variable which points to DevIL library home
DEVIL_HOME=${DEVIL_HOME:=undefined}
if [ "$DEVIL_HOME" = "undefined" ]; then
  echo "DevIL environment variable not found, setting it"
  DEVIL_HOME="$GUCEF_HOME/dependencies/Devil"
fi
echo "DEVIL_HOME = $DEVIL_HOME"


# Set environment variable which points to ZLib library home
ZLIB_HOME=${ZLIB_HOME:=undefined}
if [ "$ZLIB_HOME" = "undefined" ]; then
  echo "ZLib environment variable not found, setting it"
  ZLIB_HOME="$GUCEF_HOME/dependencies/zlib"
fi
echo "ZLIB_HOME = $ZLIB_HOME"


# Set environment variable which points to ZZipLib library home
ZZIPLIB_HOME=${ZZIPLIB_HOME:=undefined}
if [ "$ZZIPLIB_HOME" = "undefined" ]; then
  echo "ZZipLib environment variable not found, setting it"
  ZZIPLIB_HOME="$GUCEF_HOME/dependencies/zziplib"
fi
echo "ZZIPLIB_HOME = $ZZIPLIB_HOME"

# Check to see if we need to invoke the Project generator
SKIP_GUCEF_PREMAKE4FILEGENERATION=${SKIP_GUCEF_PREMAKE4FILEGENERATION:=undefined}
if [ "$SKIP_GUCEF_PREMAKE4FILEGENERATION" = "undefined" ];
then 
  echo "*** Generate Premake4 files ***"
  cd "$PREMAKE4COMMON_SCRIPTSTARTDIR"
  . GeneratePremake4Info.sh
else 
  echo "Skipping GUCEF's Premake4 file generation"
fi


# Go back to where we came from
cd "$PREMAKE4COMMON_SCRIPTSTARTDIR"

