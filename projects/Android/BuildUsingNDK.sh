#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

#------------------------------------------------------------------------------

function SetGucefHome {

  # Set GUCEF_HOME if it is not set yet
  GUCEF_HOME=${GUCEF_HOME:=undefined}
  if [ "$GUCEF_HOME" = "undefined" ]; then
    echo "GUCEF_HOME environment variable not found, setting it"
    GUCEF_HOME="$GUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

echo " "
echo "*** Setting up basic variables ***"
echo " "

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
GUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR=${scriptPath%/*}
echo "GUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR = $GUCEFANDROIDGLUEBUILD_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

if [ "$GUCEF_HOME" = "undefined" ]; then
  echo "GUCEF_HOME environment variable not found, fatal error"
  exit
fi

echo " "
echo "*** Locating NDK ***"
echo " "

. $GUCEF_HOME/projects/Android/FindNDK.sh

echo " "
echo "Changing current directory to Android project dir relative to GUCEF_HOME"

cd $GUCEF_HOME/projects/Android/jni

echo "Project dir = $PWD"
echo " "

echo " "
echo "*** Invoking build ***"
echo " "

$ANDROIDNDK/ndk-build

#echo "Press enter to continue..."
#line=""
#read line
